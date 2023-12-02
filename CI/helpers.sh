#!/bin/bash

pushd () 
{
    command pushd "$@" > /dev/null
}

popd () 
{
    command popd "$@" > /dev/null
}

check_for_keyword() 
{
    local file="$1"
    local search_string="$2"

    # Check if the file exists
    if [ ! -e "$file" ]; then
        echo "Error: File $file does not exist."
        return 1
    fi

    # Check if the search string exists in the file
    if grep -qF "$search_string" "$file"; then
        # Remove the line containing the search string
        sed -i "/$search_string/d" "$file"
        #echo "Line containing '$search_string' removed from $file."
        return 0
    else
        #echo "String '$search_string' not found in $file."
        return 1
    fi
}