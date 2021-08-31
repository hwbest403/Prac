#-*- coding: utf-8 -*-
import datetime
import time
import sys
import MeCab
import operator
from pymongo import MongoClient
from bson import ObjectId
from itertools import combinations
import re


def printMenu():
    print "1. WordCount"
    print "2. TF-IDF"
    print "3. Similarity"
    print "4. MorpAnalysis"
    print "5. CopyData"

#In this project, we assume a word seperated by a space is a morpheme.
def MorphAnalysis(docs, col_tfidf):
	print("MorpAnalysis")

	# Step(1) Read stopword list from file named stopwrod_list.txt
	stop_word = {}
	f = open("stopword_list.txt", "r")
	while True:
		line = f.readline()
		if not line: break
		stop_word[line.strip('\n')] = line.strip('\n')
	f.close()

	# Step(2) Anlaysis Morpheme in given text and delete stopword
	for doc in docs:
		content = doc['text']
		#Delete non-alphabetical characters
		content = re.sub('[^a-zA-Z]', ' ', content)
		#change all capital letter to small letter
		content = content.lower().split()

		#delete stopword in a given text dataset
		MorpList = []

		for arg in content:
			if not arg in stop_word:
				MorpList.append(arg)
	# Step(3) Store processed morpheme data into MongoDB
		col_tfidf.update({'_id':doc['_id']}, {'$set': {'morph': MorpList}}, True)

def WordCount(docs, col_tfidf):
	print("WordCount")
	#TO-DO

def TfIdf(docs, col_tfidf):
	print("TF-IDF")
    	#TO-DO

def Similarity(docs, col_tfidf):
	print("Similiarity")
	#TO-DO

def copyData(docs, col_tfidf):
	col_tfidf.drop()
	for doc in docs:
		contentDic = {}
		for key in doc.keys():
			if key != "_id":
				contentDic[key] = doc[key]
		col_tfidf.insert(contentDic)

#Access MongoDB
conn = MongoClient('localhost')

#fill it with your DB name - db+studentID ex) db20120121
db = conn['db120200192']

#fill it with your MongoDB( db + Student ID) ID and Password(default : 1234)
db.authenticate('db120200192', 'dbdb1234')

col = db['tweet']
col_tfidf = db['tweet_tfidf']

if __name__ == "__main__":
	printMenu()
	selector = input()

	if selector == 1:
        	WordCount(docs, col_tfidf)

	elif selector == 2:
        	TfIdf(docs, col_tfidf)

	elif selector == 3:
		Similarity(docs, col_tfidf)

	elif selector == 4:
		docs = col_tfidf.find()
		MorphAnalysis(docs, col_tfidf)

	elif selector == 5:
		docs = col.find()
		copyData(docs,col_tfidf)