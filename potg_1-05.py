'''
Version 1.05
	- added triggerListFlat and triggerListScale
	- openLog: nothing
	- getLocations: changed tracking whether there was a kill to whether there were points that round
	- getDescriptorLowHigh: nothing
	- getProgressDescriptor: added more conditions that will return True because points not kills now
	- getNamesAndKills: changed to 'getNamesAndLocations', only returns person and location (removed kills),
		added list to match what trigger key was in the line
	- added calculatePoints function to tally up points instead of kills to calculate potg
	- getTimes: nothing
	- findPotG: changed it to calculate points within 10 seconds instead of kills
	- outputToCSV: changed output final name to change with file name, 
'''

import sys
#this is for the log file that'll go with the code
import os
#for changing folder locations

triggerListFlat = {'killed' : 25,			#flat
					'kill assist' : 10,		#flat
					'jarate_attack' : 5,	#flat
					'killedobject' : 5}		#flat

triggerListScale = {'healed' : 1,				#scale
					'damage' : 2,				#scale
					'medic_death_ex' : 0.02}	#scale

def openLog(fileName):
	fin = open(fileName, mode='r')
	#opens the log file for reading
	allLines = fin.readlines()
	#takes in all the lines
	fin.close()
	#closes the file
	return allLines
	#return the list with all the lines of the log file
#end openLog

def getLocations(allLines):
	roundInProgress = False
	#true between round start and round win
	locations = []
	#list with the indeces of points
	locationDescription = []
	#lists what the point was so I don't have to redo it later
	lineNumber = 0
	for line in allLines:
		(low, high) = getDescriptorLowHigh(line)
		#helper function to get the low/high values
		#unpack tuple into 2 variables
		worldTrigger = False
		#fix for world trigger and team lines fucking it up
		
		if roundInProgress:
			(descriptor, lineNumBool, rIP) = getProgressDescriptor(line, worldTrigger, roundInProgress, low, high)
			#unpack tuple, returns tuple of whether there were points and updating roundInProgress
			roundInProgress = rIP
			#updatinag roundInProgress
			if lineNumBool:
				locations.append(lineNumber)
				#adds the line to the list if there were points that line
				locationDescription.append(descriptor)
				#adds the name of what point they got
		
		if line[low:high+22] == 'World triggered "Round_Start"':
			roundInProgress = True
			#changes roundInProgress to True so that the if statement above executes
		lineNumber += 1
	return (locations, locationDescription)
#end getLocations

def getDescriptorLowHigh(line):	#getLocations helper function
	low = 0
	high = 7
	
	worldTrigger = False
	
	while (line[low:high] != '<Blue>"' and line[low:high] != '><Red>"' and
			line[low:high] != 'World t' and line[low:high] != 'known>"' and
			line[low:high] != 'Team "R' and line[low:high] != 'Team "B'):
		#goes up to the point in the line where the descriptor is or
		#when its a world or team line it stops
		
		low += 1
		high += 1
	return (low,high)
#end getDescriptor

def getProgressDescriptor(line, worldTrigger, roundInProgress, low, high):	#getLocations helper function
	pointsBool = False
	descriptor = ''
	try:	
		if line[high-1] == 't' or line[high-1] == 'R' or line[high-1] == 'B':
			worldTrigger = True
		#for when it tries to compare an int to a char
	except TypeError:
		pass
		#just continues on when it encounters this error
	if(not worldTrigger):
		high += 1
		while line[high] != '"':
			descriptor += line[high]
			high += 1
		descriptor = descriptor[:-1]
		#goes til the '"' after the descriptor so need to take the '"'
		#off of the end
		
		if descriptor == 'killed':
			pointsBool = True
			#most kills are under killed and not triggered
		elif descriptor == 'triggered':
			secondary = ''
			#gets the word(s) after triggered in quotes
			high += 1
			while line[high] != '"':
				secondary += line[high]
				high += 1
			if secondary in triggerListFlat or secondary in triggerListScale:
				pointsBool = True
				#this will contain only the lines 
			descriptor = secondary
	else:
		if line[low:high+20] == 'World triggered "Round_Win"':
			roundInProgress = False
	return (descriptor, pointsBool,roundInProgress)
#end getProgressDescriptor

def getNamesAndLocations(allLines, locations, locationDescription):
	names = []
	#string list of names
	personalLocations = []
	#list of lists of where each name's points are
	locationDescriptionList = []
	
	for line in range(len(locations)):
		low = 0
		while allLines[locations[line]][low] != '"':
			low += 1
		low += 1
		try:
			high = allLines[locations[line]].index('><[U:')
			#finds the start location of this strng in the line
			name = allLines[locations[line]][low:high-2]
			if name[len(name)-1] == '<':
				name = name[:-1]
			#depends on whether the number assigned to the player was a single digit or double digit
			#so this catches in case there are 2 digits assigned to them
			
			try:
				namesIndex = names.index(name)
				#return index of name if it is in the list, otherwise it goes down to except
				
				personalLocations[namesIndex].append(locations[line])
				locationDescriptionList[namesIndex].append(locationDescription[line])
			except ValueError:
				names.append(name)
				personalLocations.append([])
				personalLocations[len(personalLocations)-1].append(locations[line])
				locationDescriptionList.append([])
				locationDescriptionList[len(locationDescriptionList)-1].append(locationDescription[line])
		except ValueError:
			pass
	return (names, personalLocations, locationDescriptionList)	
#end getNamesAndKills

def calculatePoints(allLines, personalLocations, locationDescriptionList):
	points = []
	for person in range(len(personalLocations)):
		points.append([])
		for pointLine in range(len(personalLocations[person])):
			
			if locationDescriptionList[person][pointLine] in triggerListFlat:
				points[person].append(triggerListFlat[locationDescriptionList[person][pointLine]])
			else:	#if not in flat, it'll be in scale list
				amount = ''
				#the number of heal, damage, or ubercharge
				amountString = ''
				if (locationDescriptionList[person][pointLine] == 'healed' or
						locationDescriptionList[person][pointLine] == 'medic_death_ex'):
					amountString = allLines[personalLocations[person][pointLine]][-7:]
					#gets the end of the line where the heal info is located
				elif locationDescriptionList[person][pointLine] == 'damage':
					(low, high) = getDescriptorLowHigh(allLines[personalLocations[person][pointLine]])
					#high will be where the first person's name ends
					amountString = allLines[personalLocations[person][pointLine]][high:]
					
					(low, high) = getDescriptorLowHigh(amountString)
					#will return the end of the second player's name, which is where the damage starts
					amountString = amountString[high:]
					#time to parse
				else:
					print('you done fucked up')
				
				quoteCount = 0
				#heal amount is between quotes
				stringCount = 0
				#to parse the string
				if '(realdamage "' in amountString:
					quoteCount -= 2
				while quoteCount < 2 and stringCount < len(amountString):
					if quoteCount == 1:
						amount += amountString[stringCount]
					if amountString[stringCount] == '"':
						quoteCount += 1
					stringCount += 1
				amount = int(amount[:-1])
				
				if locationDescriptionList[person][pointLine] == 'healed':
					points[person].append(amount*triggerListScale['healed'])
				elif locationDescriptionList[person][pointLine] == 'medic_death_ex':
					points[person].append(25*(1+(amount*triggerListScale['medic_death_ex'])))
				elif locationDescriptionList[person][pointLine] == 'damage':
					points[person].append(amount*triggerListScale['damage'])
				else:
					print('it\'s still fucked up')
	return points
#end calculatePoints

def getTimes(allLines, names, personalLocations):
	personalTimes = []
	#list of lists for the time of each person's kill(s)
	
	for i in range(len(names)):
		personalTimes.append([])
		#adds a list for this person's kills
		for j in range(len(personalLocations[i])):
			personalTimes[i].append(allLines[personalLocations[i][j]][15:23])
			#adds the time to this person's list
	return personalTimes
#end getTimes

def findPotG(personalTimes, points):
	bestPerson = 0
	#person with most kills in 10 sec time period
	highestPoints = 0
	#most points in 10 sec time period
	timeOfPotG = ''
	#string of the time when potg occurs
	for i in range(len(personalTimes)):
	#number of people
		for j in range(len(personalTimes[i])):
		#number of times when they got points
			k = j+1
			#for comparing times
			tempHighPoints = float(points[i][j])
			while k<len(personalTimes[i]):
			#to keep k in index
				resetPT = personalTimes[i][k]
				#variable to reset the times later on
				if int(personalTimes[i][k][6:]) < int(personalTimes[i][j][6:]):
					#k will always be higher, so this option is to add 60 for the subtraction
					personalTimes[i][k] = personalTimes[i][k][0:3] + str(int(personalTimes[i][k][3:5])-1) + ':' + str(int(personalTimes[i][k][6:])+60)
					#this clusterfuck of a line subtracts 1 from minutes and adds 60 to seconds
				if (int(personalTimes[i][k][3:5]) - int(personalTimes[i][j][3:5]) == 0 and
						int(personalTimes[i][k][6:]) - int(personalTimes[i][j][6:]) <= 10):
					tempHighPoints += float(points[i][k])
					#the point was within 10 seconds of the original
					if (tempHighPoints > highestPoints):
						highestPoints = tempHighPoints
						bestPerson = i
						timeOfPotG = personalTimes[i][j]
						#overwrites the values if they beat the current best
					personalTimes[i][k] = resetPT
				else:
					personalTimes[i][k] = resetPT
					#too far away so break
					break
				k += 1
	return (bestPerson, highestPoints, timeOfPotG)
#end findPotG

def outputToCSV(names, points, bestPerson, highestPoints, timeOfPotG):
	fileName = (__file__)[:-3] + '_points_' + sys.argv[1][4:-4] + '.csv'
	fout = open(fileName,'w')
	#opens/makes file (encoding='utf8' may be optional, i'm not sure)
	for i in range(len(points)):
		totPoints = 0
		for j in range(len(points[i])):
			totPoints += points[i][j]
		fout.write(names[i] + ',' + str(totPoints) + '\n')
	fout.write('PotG,' + names[bestPerson] + ',Points: ' + str(highestPoints) + ',time:' + timeOfPotG)
	
	fout.close()
	return
#end outputToCSV

#---------- ---------- main ---------- ----------

allLines = openLog(sys.argv[1])
#sys.argv[1] will be the log file when running the python code
#ex.	>>$python potg.py log_***.log

(locations, locationDescriptions) = getLocations(allLines)
#returns a tuple with the indeces of all points and what they were

(names, personalLocations, locationDescriptionList) = getNamesAndLocations(allLines, locations, locationDescriptions)
#returns a tuple with 2 lists for names and locations of points

points = calculatePoints(allLines, personalLocations, locationDescriptionList)
#returns a list with points indexed like the locationList

personalTimes = getTimes(allLines, names, personalLocations)
#gets the time of all points

(bestPerson, highestPoints, timeOfPotG) = findPotG(personalTimes, points)
#gets the index and count of the potg

outputToCSV(names, points, bestPerson, highestPoints, timeOfPotG)
#outputs the names and kills to a csv file

#---------- ---------- end main ---------- ----------





