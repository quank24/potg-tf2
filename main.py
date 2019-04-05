import os
import sys

dir = os.getcwd()
os.path.append(os.path.abspath(dir+'src/'))
import potg

all_lines = openLog(sys.argv[1])
#sys.argv[1] will be the log file when running the python code
#ex.	>>$python main.py <LOG_NAME>.log

(locations, location_descriptions) = getLocations(all_lines)
#returns a tuple with the indeces of all points and what they were

(names, personal_locations, location_description_list) = getNamesAndLocations(all_lines, locations, location_descriptions)
#returns a tuple with 2 lists for names and locations of points

points = calculatePoints(all_lines, personal_locations, location_description_list)
#returns a list with points indexed like the locationList

personal_times = getTimes(all_lines, names, personal_locations)
#gets the time of all points

(best_person, highest_points, time_of_potg) = findPotG(personal_times, points)
#gets the index and count of the potg

outputToCSV(names, points, best_person, highest_points, time_of_potg)
#outputs the names and kills to a csv file
