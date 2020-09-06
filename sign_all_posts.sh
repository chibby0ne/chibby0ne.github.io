#!/bin/bash

for post in content/posts/*.md; do
    postfile=$(basename ${post})
    signature=_posts_signatures/${postfile%%.md}.asc
    if [[ ${post} -nt ${signature} ]]; then
        echo "post: ${postfile} was modified after the signature creation or signature doesn't exist yet, therefore need to (re)generate that post's signature"
        keybase pgp sign -i $post > ${signature}
    else
        echo "post: ${postfile} has NOT been modified after the signature creation. NO regeneration of signature needed"
    fi
done
