#-*- coding: utf-8 -*-
import datetime
import time
import sys
import MeCab
import operator
from pymongo import MongoClient
from bson.objectid import ObjectId
from itertools import combinations
import re
import pymongo
import pprint
import math


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

	searchid = raw_input("Insert objectID for search Morph : ")

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
		
	search = col_tfidf.find_one({'_id':ObjectId(searchid)})
	searchmorp = search['morph']
	print(searchmorp)

def WordCount(docs, col_tfidf):
	print("WordCount")
	#TO-DO

	searchid = raw_input("Insert objectID for search Wordcount : ")	

	for doc in docs:
		Morp = doc['morph']

		Morpcount=[]

		for arg in Morp:
			Morpcount.append(Morp.count(arg))
		
		col_tfidf.update({'_id':doc['_id']}, {'$set': {'wordcount':Morpcount}}, True)

	search = col_tfidf.find_one({'_id':ObjectId(searchid)})
	searchmorp = search['morph']
	searchwc = search['wordcount']
	print(searchmorp)
	print(searchwc)

def TfIdf(docs, col_tfidf):
	print("TF-IDF")
    	#TO-DO

	searchid = raw_input("Insert objectID for search TF-IDF : ")
	
	docsum = col_tfidf.count()
	clone_docs = docs.clone()

	
	for doc in docs:
		clone_docs.rewind()
		Wordcount = doc['wordcount']
		doc_morph = doc['morph']

		TFIDF = []
		all_cnt = {}
		for morph in doc['morph']:
			clone_docs.rewind()
			cnt = 0
			for temp_doc in clone_docs:
				if morph in temp_doc['morph']:
					cnt += 1
			all_cnt[morph] = cnt

		for idx, arg in enumerate(Wordcount):
			length = float(len(Wordcount))
			TF = float(arg)/length
			IDF = math.log(float(docsum)/all_cnt[doc_morph[idx]])
			TI = round(TF * IDF, 4)
			TFIDF.append(TI)

		col_tfidf.update({'_id':doc['_id']}, {'$set': {'TFIDF':TFIDF}}, True)
	
	search = col_tfidf.find_one({'_id':ObjectId(searchid)})
	searchmorp = search['morph']
	searchtf = search['TFIDF']
	print(searchmorp)
	print(searchtf)

def Similarity(docs, col_tfidf):
	print("Similiarity")
	#TO-DO

	first = raw_input("Insert first objectID for search Similarity : ")
	second = raw_input("Insert second objectID for search Similarity : ")

	firstmorp = col_tfidf.find_one({'_id':ObjectId(first)})
	secondmorp = col_tfidf.find_one({'_id':ObjectId(second)})
	
	first_data = dict(zip(firstmorp['morph'], firstmorp['TFIDF']))
	first_sum = sum([x * x for x in firstmorp['TFIDF']])

	second_data = dict(zip(secondmorp['morph'], secondmorp['TFIDF']))
	second_sum = sum([x * x for x in secondmorp['TFIDF']])

	morph_set = set(firstmorp['morph']+secondmorp['morph'])


	value = sum([first_data.get(x, 0.0)*second_data.get(x, 0.0) for x in morph_set])

	similarity = value / (math.sqrt(first_sum) * math.sqrt(second_sum))
	print(similarity)
	return similarity
	

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
db = conn['db20171273']

#fill it with your MongoDB( db + Student ID) ID and Password(default : 1234)
db.authenticate('db20171273', '1234')

col = db['tweet']
col_tfidf = db['tweet_tfidf']

if __name__ == "__main__":
	printMenu()
	selector = input()

	if selector == 1:
		docs=col_tfidf.find()
        	WordCount(docs, col_tfidf)

	elif selector == 2:
		docs = col_tfidf.find()
        	TfIdf(docs, col_tfidf)

	elif selector == 3:
		docs = col_tfidf.find()
		Similarity(docs, col_tfidf)

	elif selector == 4:
		docs = col_tfidf.find()
		MorphAnalysis(docs, col_tfidf)

	elif selector == 5:
		docs = col.find()
		copyData(docs,col_tfidf)
