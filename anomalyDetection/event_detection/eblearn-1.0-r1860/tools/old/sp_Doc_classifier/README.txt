To compile this project, you need :
- libidx
- eblearn
- libspidx

you also need the library Z (to manipulate compressed datas)

To get the datas used for this project :
go to http://leon.bottou.org/projects/sgd
get :
- lyrl2004_tokens_test_pt0.dat.gz,
- lyrl2004_tokens_test_pt1.dat.gz,
- lyrl2004_tokens_test_pt2.dat.gz
- lyrl2004_tokens_test_pt3.dat.gz
- lyrl2004_tokens_train.dat.gz
- rcv1-v2.topics.qrels.gz

Put it in a repertory (in "data" for example)

Then in a shell, run from eblearn/bin :
./sp_doc_classifier path/to/the/data true

path/to/the/data would be ../eblearn/demos/sp_Dox_classifier/data (no "/" at the end !)
if you put it in the data repertory
true is for preprocessing, you wont need it after the first call

After preprocessing, you will have 2 new files, "test.dat.gz" and "train.dat.gz", in
your data repertory. You won't need the downloaded files anymore

WARNING : this is just a toy example for demo purpose. It has not been tweaked for 
optimal results, and playing with the various variables should easily get you better
results than the ones currently obtained

THANKS : to Leon Bottou for the datas and the preprocessing code