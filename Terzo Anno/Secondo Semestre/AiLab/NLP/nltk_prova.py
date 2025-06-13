from nltk.tokenize import word_tokenize, sent_tokenize
from nltk.corpus import stopwords
import nltk
from nltk.stem import WordNetLemmatizer
from nltk.stem import PorterStemmer



example_string= "The crew of the USS Discovery discovered many discoveries. Dicovery is great."

s_tokenized= sent_tokenize(example_string)
w_tokenized= word_tokenize(example_string)
#print(s_tokenized)
#print(w_tokenized)

words= set(word_tokenize(example_string))

#set the languae for the stopwords

stop_words= set(stopwords.words('english'))
filtered_list= []
for w in w_tokenized:
    if w not in stop_words:
        filtered_list.append(w)
#print(filtered_list)


#stemming, reducing words to their root form

ps= PorterStemmer()
stemmed_list= []
for w in filtered_list:
    stemmed_list.append(ps.stem(w))
#print(stemmed_list)

#part of speech tagging
tagged_list= pos_tag(words)

#lemmatizing
lemmatizer= WordNetLemmatizer()
lemmmm= lemmatizer.lemmatize("scarves")
print(lemmmm)