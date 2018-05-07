#!/bin/bash

for post in _posts/*.md; do
    postfile=$(basename ${post})
    keybase pgp sign -i $post > _posts_signatures/${postfile%%.md}.asc
done
