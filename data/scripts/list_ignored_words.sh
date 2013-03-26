#!/bin/bash
path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$path"

dbfile="../words.db"
dbtable="ignored_words"

if [ ! -f "$dbfile" ]; then
	echo "The database file does not exist, please create it first."
	exit 0
fi

echo "The following are the actively ignored words: "
dbquery="SELECT word FROM $dbtable where ignored='TRUE'"
sqlite3 "$dbfile" "$dbquery"
