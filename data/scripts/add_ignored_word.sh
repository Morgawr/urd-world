#!/bin/bash
path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$path"

dbfile="../words.db"
dbtable="ignored_words"

if [ ! -f "$dbfile" ]; then
	echo "The database file does not exist, please create it first."
	exit 0
fi

if [ $# -eq 0 ]; then
	echo "No arguments supplied, you need to pass which word you want to ignore."
	exit 0
fi

dbword=$1

echo "Adding $dbword to list of ignored words..."
dbquery="INSERT INTO $dbtable (word,ignored) values ('$dbword', 'TRUE')"
echo $dbquery
sqlite3 "$dbfile" "$dbquery"
echo "... done."
