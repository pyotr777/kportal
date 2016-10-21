#!/bin/bash

echo "Post command" > post.log
ls -l >> post.log
echo "Image exchange" >> post.log
curl -F file=@"source.gif" http://52.198.169.97:8080/files  > response.gif 2>> post.log
ls -l >> post.log