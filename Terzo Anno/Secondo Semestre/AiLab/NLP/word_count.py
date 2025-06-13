from sklearn.feature_extraction.text import CountVectorizer, TfidfVectorizer
import pandas as pd
sample=[
    "problem of evil",
    "evil queen",
    "horizon problem"
]

#istanct of the counter
#vec= CountVectorizer()
vec= TfidfVectorizer() #tf-idf vectorizer
#fit the data
x= vec.fit_transform(sample)

#convert x yo a dataframe for easy reading
x_pd= pd.DataFrame(x.toarray(), columns=vec.get_feature_names_out())
print(x_pd)
