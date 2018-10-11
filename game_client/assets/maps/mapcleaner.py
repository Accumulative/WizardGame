import csv

mapNum = 1
sizeX = 68;
sizeY = 42;

# mapNum = 2
# sizeX = 22;
# sizeY = 17;

def cleanFile(src, dest):
	countY = 0;
	with open(src, newline='') as csvfile:
		with open(dest, 'w') as csvfile2:
			spamreader = csv.reader(csvfile, delimiter=',')
			for row in spamreader:
				if countY < sizeY: 
					if len(row) >= sizeX:
						print(','.join(row[0:sizeX]), file=csvfile2)
					else:
						print(','.join([row[a] if a < len(row) else '-1' for a in range(sizeX)] ), file=csvfile2)
				countY+=1
	countY = 0

for i in range(3):
	cleanFile('{0}/{0}_{1}.csv'.format(mapNum, i+1), '{}/{}.csv'.format(mapNum, i))

cleanFile('{0}/{0}_Collision.csv'.format(mapNum), '{}/Collision.csv'.format(mapNum))

