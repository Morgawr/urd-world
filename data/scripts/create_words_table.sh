#!/bin/bash
path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$path"

dbfile="../words.db"

if [ -f "$dbfile" ]; then
	echo "The file words.db already exit, this will erase any previous database."
	read -p "Are you sure you want to proceed? [Y/N]: " -n 1
	echo 
	if [[ ! $REPLY =~ ^[Yy]$ ]]; then
		echo "Process aborted successfully."
		exit 1
	fi
	rm "$dbfile"
fi

echo "The words.db database will be created now..."

echo "... creating table of ignored words ..."
sqlite3 "$dbfile" "CREATE TABLE ignored_words (id INTEGER PRIMARY KEY, word TEXT, ignored BOOL)"

echo "Database of words successfully created!"
