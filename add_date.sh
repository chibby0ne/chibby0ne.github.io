#!/usr/bin/env bash

files=$(ls -1 content/posts/*.md)

for f in ${files}; do
    filename=$(basename ${f})
    # if [[ ${filename} != "2020-05-04-firefox_screen_tearing_while_scrolling.md" ]]; then
    #     echo skipping file...
    #     continue
    # fi
    date=$(expr "${filename}" : '\([0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\}\)')
    sed -i "s/title:/date: ${date}\n&/g" ${f}
done

