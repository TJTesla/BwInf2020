import sys


class SearchedWord:
    def __init__(self, newWord, index):
        self.word = newWord  # Declaration of attributes of the class (Instance attributes)
        self.punctuation = ''
        self.completedWord = ''
        self.givenChars = {}
        self.origIndex = index

        punctuationMarks = [',', '.', ';', ':', '!', '?']
        if self.word[len(self.word) - 1] in punctuationMarks:  # Delete last char if it is a punctuation mark
            self.punctuation = self.word[len(self.word) - 1]
            self.word = self.word[:-1]

        for letter in range(len(self.word)):  # Look for given letters
            if self.word[letter] != '_':
                self.givenChars[letter] = self.word[letter]


def fillWordLists(missing, given):
    path = './examples/'
    if len(sys.argv) == 1:
        path = path + 'raetsel0.txt'
    else:
        path = path + sys.argv[1]

    textInput = open(path, 'r')

    missedWordsLine = textInput.readline()  # First line of file
    missedWords = missedWordsLine.split()  # Array of words from first line
    for wordIterator in range(len(missedWords)):  # Fills Array
        missedWords[wordIterator].strip()
        missing.append(SearchedWord(missedWords[wordIterator], wordIterator))

    givenWordsLine = textInput.readline()  # Essentially same as above
    givenTemp = givenWordsLine.split()
    for wordIterator in givenTemp:
        wordIterator.strip()
        given.append(wordIterator)


def wouldFit(hidden, openned):
    if len(hidden.word) != len(openned):
        return False
    if len(list(hidden.givenChars)) == 0:
        return True
    for keyChar in hidden.givenChars:
        givenChar = openned[keyChar]
        searchedChar = hidden.givenChars[keyChar]
        if givenChar != searchedChar:
            return False
        else:
            continue
    return True


def printFinished(searchedItem):
    for iterate in range(len(searchedItem)):  # Sorting w/ the index => Getting everything in correct order
        for jIter in range(0, len(searchedItem) - iterate - 1):
            if searchedItem[jIter].origIndex > searchedItem[jIter + 1].origIndex:
                searchedItem[jIter], searchedItem[jIter + 1] = searchedItem[jIter + 1], searchedItem[jIter]

    for searchedWord in searchedItem:
        print(searchedWord.completedWord, searchedWord.punctuation, sep='', end=' ')
    print('')


def recursiveSolution(searched, given, cat):
    for iterate in range(len(given[cat]) - 1, -1, -1):  # Iteration through words in a category
        counter = 0
        hiddenIndex = int
        for jIter in range(len(searched[cat])):  # Check w/ every hidden word if it would match
            if wouldFit(searched[cat][jIter], given[cat][iterate]) and searched[cat][jIter].completedWord == '':
                counter += 1
                hiddenIndex = jIter
        if counter == 1:
            searched[cat][hiddenIndex].completedWord = given[cat][iterate]
            del given[cat][iterate]
            continue
    if len(given[cat]) != 0:  # See notes for logical reference
        fixDuplicateWords(searched, given, cat)
        recursiveSolution(searched, given, cat)
    return


def fixDuplicateWords(searched, given, cat):
    duplicateMem = []
    alreadyUsed = []
    for iterate in range(len(given[cat])):  # Save occurences of duplicate word in array
        duplicateMem.append([])  # First Index: Position of word in given[cat]
        duplicateMem[iterate].append(iterate)  # Second Index: Positions of duplicates
        for jIter in range(iterate+1, len(given[cat])):  # Duplicates aren't written twice...
            if given[cat][iterate] == given[cat][jIter] and jIter not in alreadyUsed:
                duplicateMem[iterate].append(jIter)  # ... (i.e. [[0,2], [1], [0, 2]] would not occur
                alreadyUsed.append(jIter)  # It would be: [[0,2], [1], [2]]

    for subArr in range(len(duplicateMem)-1, -1, -1):  # deleting everything that doen't have a duplicate
        if len(duplicateMem[subArr]) == 1:
            del duplicateMem[subArr]

    for iterate in range(len(duplicateMem) - 1, -1, -1):  # Iteration through duplicate words
        counter = 0
        hiddenIndex = []
        for jIter in range(len(searched[cat])):  # Check w/ every hidden word if it would match
            if wouldFit(searched[cat][jIter], given[cat][duplicateMem[iterate][0]]) and searched[cat][jIter].completedWord == '':
                counter += 1
                hiddenIndex.append(jIter)
        if counter == len(duplicateMem[iterate]):  # Check if there are as many possibilities as duplicate words
            for duplicateWord in range(len(duplicateMem[iterate])-1, -1, -1):  # For every word, mark as finished...
                # searched[cat][duplicateMem[iterate][duplicateWord]].completedWord = given[cat][duplicateMem[iterate][duplicateWord]]
                searched[cat][hiddenIndex[duplicateWord]].completedWord = given[cat][duplicateMem[iterate][duplicateWord]]
                del given[cat][duplicateMem[iterate][duplicateWord]]  # ...(mark as finished) & delete

                for jIter in range(len(duplicateMem)):  # Loweres every index that is higher than the index that deletes
                    for kIter in range(len(duplicateMem[jIter])):  # Else it would point to wrong element because elements shift on eto left
                        if duplicateMem[jIter][kIter] > duplicateMem[iterate][duplicateWord]:  # But indexes would't
                            duplicateMem[jIter][kIter] -= 1


if __name__ == '__main__':
    searchedWords = []
    givenWords = []
    fillWordLists(searchedWords, givenWords)

    givenWords.sort(key=len)  # Sort list after length of words

    givenDict = {}
    currentWordLen = int
    previousWordLen = int
    begin = 0
    for word in range(1, len(givenWords)):  # Run through list; Look after positions where amount of letters changes
        currentWordLen = len(givenWords[word])  # Save 'categorie' of words with same length in dictionary
        previousWordLen = len(
            givenWords[word - 1])  # Key: Amount of letters; Value: List w/ all words w/ amount of chars

        if currentWordLen != previousWordLen:
            collector = []
            for i in range(begin, word):
                collector.append(givenWords[i])
            begin = word
            givenDict[previousWordLen] = collector

    collector = []
    for i in range(begin, len(givenWords)):
        collector.append(givenWords[i])
    givenDict[len(givenWords[begin])] = collector

    # Divider for readability

    for i in range(len(searchedWords)):  # Same as above; W/ searched words; Sorting via Bubble Sort
        for j in range(0, len(searchedWords) - i - 1):
            if len(searchedWords[j].word) > len(searchedWords[j + 1].word):
                searchedWords[j], searchedWords[j + 1] = searchedWords[j + 1], searchedWords[j]

    searchedDict = {}
    currentWordLen = int
    previousWordLen = int
    begin = 0
    for word in range(1, len(searchedWords)):
        currentWordLen = len(searchedWords[word].word)
        previousWordLen = len(searchedWords[word - 1].word)

        if currentWordLen != previousWordLen:
            collector = []
            for i in range(begin, word):
                collector.append(searchedWords[i])
            begin = word
            searchedDict[previousWordLen] = collector

    collector = []
    for i in range(begin, len(searchedWords)):
        collector.append(searchedWords[i])
    searchedDict[len(searchedWords[begin].word)] = collector

    # Start of algorithm:

    for key in givenDict:  # Iteration through categories
        recursiveSolution(searchedDict, givenDict, key)

    printFinished(searchedWords)
