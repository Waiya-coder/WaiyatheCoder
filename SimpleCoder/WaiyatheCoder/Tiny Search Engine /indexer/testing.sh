
echo "test"

#invalid arguments
echo  "Incorrect number of arguments"
./indexer  /thayerfs/home/f006g8m/cs50-dev/shared/tse/crawldata/letters-2/ index.out


#no argument
echo  "No argument"
./indexer bjbj


#one argument
echo -e "One argument"
 ./indexer  /thayerfs/home/f006g8m/cs50-dev/shared/tse/crawldata/letters-2/  



 echo -e "More than three argument"
 ./indexer  /thayerfs/home/f006g8m/cs50-dev/shared/tse/crawldata/letters-2/  index.out index

 ./indexer /thayerfs/home/f006g8m/cs50-dev/shared/tse/crawldata/letters-2/  index.out index

./indexer  /thayerfs/home/f006g8m/cs50-dev/shared/tse/crawldata/toscrape-2/  index.out index

valid arguments

 echo -e "Valid arguments"
./indexer /thayerfs/home/f006g8m/cs50-dev/shared/tse/crawldata/letters-3/  index.out 


# #indextest
echo -e "IndexTest"
 $ ~/cs50-dev/shared/tse/indexcmp
 ~/cs50-dev/shared/tse/indexcmp  /thayerfs/home/f006g8m/cs50-dev/shared/tse/crawldata/letters-2.index  index11.out



