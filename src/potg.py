# re.compile('"(.+)<\d+><\[U:\d:\d+\]><.+>" (triggered | killed)'

'''
--------- IMPORTS ---------
'''
import sys
#this is for the log file that'll go with the code
import os
#for changing folder locations

'''
--------- DICTIONARIES ---------
'''

# flat amount points
trigger_list_flat = {'killed' : 25,
					'kill assist' : 10,
					'jarate_attack' : 5,
					'killedobject' : 5}

# scaling points
trigger_list_scale = {'healed' : 1,
					'damage' : 2,
					'medic_death_ex' : 0.02}

'''
--------- FUNCTIONS ---------
'''

''' opens the log file, takes all the lines into a string list, and returns it '''
def openLog(file_name):
	fin = open(file_name, mode='r')
	# opens the log file for reading
	all_lines = fin.readlines()
	# takes in all the lines
	fin.close()
	# closes the file
	return all_lines
	# return the list with all the lines of the log file
#end openLog

'''  '''
def getDescriptorLowHigh(line):	# getLocations helper function
	low = 0
	high = 9

	# world_trigger = False
	# don't know why this line was here

	while (line[low:high] != ']><Blue>"' and line[low:high] != ']><Red>" ' and
			line[low:high] != 'World tri' and line[low:high] != ']><unknow' and
			line[low:high] != 'Team "Red' and line[low:high] != 'Team "Blu'):
		# goes up to the point in the line where the descriptor is or
		# 	when its a world or team line it stops

		low += 1
		high += 1
	return (low, high)
#end getDescriptor

'''  '''
def getProgressDescriptor(line, round_in_progress, low, high):	# getLocations helper function
	world_trigger = False
	points_bool = False
	descriptor = ''
	try:
		if line[high-1] == 't' or line[high-1] == 'R' or line[high-1] == 'B':
			world_trigger = True
		# for when it tries to compare an int to a char
	except TypeError:
		pass
		# just continues on when it encounters this error
	if(not world_trigger):
		high += 1
		while line[high] != '"':
			descriptor += line[high]
			high += 1
		descriptor = descriptor[:-1]
		# goes til the '"' after the descriptor so need to take the '"'
		# 	off of the end

		if descriptor == 'killed':
			points_bool = True
			# most kills are under killed and not triggered
		elif descriptor == 'triggered':
			secondary = ''
			# gets the word(s) after triggered in quotes
			high += 1
			while line[high] != '"':
				secondary += line[high]
				high += 1
			if secondary in trigger_list_flat or secondary in trigger_list_scale:
				points_bool = True
				# this will contain only the lines
			descriptor = secondary
	else:
		if line[low:high+20] == 'World triggered "Round_Win"':
			round_in_progress = False
	return (descriptor, points_bool, round_in_progress)
#end getProgressDescriptor

'''  '''
def getLocations(all_lines):
	round_in_progress = False
	# true between round start and round win
	locations = []
	# list with the indeces of points
	location_description = []
	# lists what the point was, so I don't have to redo it later
	line_number = 0
	for line in all_lines:
		(low, high) = getDescriptorLowHigh(line)
		# helper function to get the low/high values
		# 	unpack tuple into 2 variables
		
		if round_in_progress:
			(descriptor, line_num_bool, new_rip) = getProgressDescriptor(line, round_in_progress, low, high)
			# unpack tuple, returns tuple of whether there were points and updating roundInProgress
			round_in_progress = new_rip
			# updating roundInProgress
			if line_num_bool:
				locations.append(line_number)
				# adds the line to the list if there were points that line
				location_description.append(descriptor)
				# adds the name of what point they got

		if line[low:high+22] == 'World triggered "Round_Start"':
			round_in_progress = True
			# changes roundInProgress to True so that the if statement above executes
		line_number += 1
	return (locations, location_description)
#end getLocations

'''  '''
def getNamesAndLocations(all_lines, locations, location_description):
	names = []
	# string list of names
	personal_locations = []
	# list of lists of where each name's points are
	location_description_list = []

	for line in range(len(locations)):
		low = 0
		while all_lines[locations[line]][low] != '"':
			low += 1
		low += 1
		try:
			high = all_lines[locations[line]].index('><[U:')
			# finds the start location of this strng in the line
			name = all_lines[locations[line]][low:high-2]
			if name[len(name)-1] == '<':
				name = name[:-1]
			# depends on whether the number assigned to the player was a single digit or double digit
			# 	so this catches in case there are 2 digits assigned to them

			try:
				names_index = names.index(name)
				# return index of name if it is in the list, otherwise it goes down to except

				personal_locations[names_index].append(locations[line])
				location_description_list[names_index].append(location_description[line])
			except ValueError:
				names.append(name)
				personal_locations.append([])
				personal_locations[len(personal_locations)-1].append(locations[line])
				location_description_list.append([])
				location_description_list[len(location_description_list)-1].append(location_description[line])
		except ValueError:
			pass
	return (names, personal_locations, location_description_list)
#end getNamesAndKills

'''  '''
def calculatePoints(all_lines, personal_locations, location_description_list):
	points = []
	for person in range(len(personal_locations)):
		points.append([])
		for point_line in range(len(personal_locations[person])):

			if location_description_list[person][point_line] in trigger_list_flat:
				points[person].append(trigger_list_flat[location_description_list[person][point_line]])
			else:	# if not in flat, it'll be in scale list
				amount = ''
				# the number of heal, damage, or ubercharge
				amount_string = ''
				if (location_description_list[person][point_line] == 'healed' or
						location_description_list[person][point_line] == 'medic_death_ex'):
					amount_string = all_lines[personal_locations[person][point_line]][-7:]
					# gets the end of the line where the heal info is located
				elif location_description_list[person][point_line] == 'damage':
					(low, high) = getDescriptorLowHigh(all_lines[personal_locations[person][point_line]])
					# high will be where the first person's name ends
					amount_string = all_lines[personal_locations[person][point_line]][high:]

					(low, high) = getDescriptorLowHigh(amount_string)
					# will return the end of the second player's name, which is where the damage starts
					amount_string = amount_string[high:]
					# time to parse
				else:
					print('you done fucked up')

				quote_count = 0
				# heal amount is between quotes
				string_count = 0
				# to parse the string
				if '(realdamage "' in amount_string:
					quote_count -= 2
				while quote_count < 2 and string_count < len(amount_string):
					if quote_count == 1:
						amount += amount_string[string_count]
					if amount_string[string_count] == '"':
						quote_count += 1
					string_count += 1
				amount = int(amount[:-1])

				if location_description_list[person][point_line] == 'healed':
					points[person].append(amount*trigger_list_scale['healed'])
				elif location_description_list[person][point_line] == 'medic_death_ex':
					points[person].append(25*(1+(amount*trigger_list_scale['medic_death_ex'])))
				elif location_description_list[person][point_line] == 'damage':
					points[person].append(amount*trigger_list_scale['damage'])
				else:
					print('it\'s still fucked up')
	return points
#end calculatePoints

'''  '''
def getTimes(all_lines, names, personal_locations):
	personal_times = []
	# list of lists for the time of each person's kill(s)

	for i in range(len(names)):
		personal_times.append([])
		# adds a list for this person's kills
		for j in range(len(personal_locations[i])):
			personal_times[i].append(all_lines[personal_locations[i][j]][15:23])
			# adds the time to this person's list
	return personal_times
#end getTimes

def findPotG(personal_times, points):
	best_person = 0
	# person with most kills in 10 sec time period
	highest_points = 0
	# most points in 10 sec time period
	time_of_potg = ''
	# string of the time when potg occurs
	for i in range(len(personal_times)):
	# number of people
		for j in range(len(personal_times[i])):
		# number of times when they got points
			k = j+1
			# for comparing times
			temp_high_points = float(points[i][j])
			while k<len(personal_times[i]):
			# to keep k in index
				reset_personal_times = personal_times[i][k]
				# variable to reset the times later on
				if int(personal_times[i][k][6:]) < int(personal_times[i][j][6:]):
					# k will always be higher, so this option is to add 60 for the subtraction
					personal_times[i][k] = personal_times[i][k][0:3] + str(int(personal_times[i][k][3:5])-1) + ':' + str(int(personal_times[i][k][6:])+60)
					# this clusterfuck of a line subtracts 1 from minutes and adds 60 to seconds
				if (int(personal_times[i][k][3:5]) - int(personal_times[i][j][3:5]) == 0 and
						int(personal_times[i][k][6:]) - int(personal_times[i][j][6:]) <= 10):
					temp_high_points += float(points[i][k])
					# the point was within 10 seconds of the original
					if (temp_high_points > highest_points):
						highest_points = temp_high_points
						best_person = i
						time_of_potg = personal_times[i][j]
						# overwrites the values if they beat the current best
					personal_times[i][k] = reset_personal_times
				else:
					personal_times[i][k] = reset_personal_times
					# too far away so break
					break
				k += 1
	return (best_person, highest_points, time_of_potg)
#end findPotG

def outputToCSV(names, points, best_person, highest_points, time_of_potg):
	file_name = (__file__)[:-3] + '_points_' + sys.argv[1][4:-4] + '.csv'
	fout = open(file_name,'w')
	# opens/makes file (encoding='utf8' may be optional, i'm not sure)
	for i in range(len(points)):
		total_points = 0
		for j in range(len(points[i])):
			total_points += points[i][j]
		fout.write(names[i] + ',' + str(total_points) + '\n')
	fout.write('PotG,' + names[best_person] + ',Points: ' + str(highest_points) + ',time:' + time_of_potg)

	fout.close()
	return
#end outputToCSV
