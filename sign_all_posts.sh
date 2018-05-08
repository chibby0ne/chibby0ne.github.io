#!/bin/bash

for post in _posts/*.md; do
    postfile=$(basename ${post})
    if [[ ! -f  _posts_signatures/${postfile%%.md}.asc ]]; then
        keybase pgp sign -i $post > _posts_signatures/${postfile%%.md}.asc
    fi
done
