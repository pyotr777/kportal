#!/bin/bash
# Start webint for installing K-portal.

echo "tmux commands"
tmux new -s kpinstall -d
tmux send -t kpinstall "cd \$HOME/kportal" ENTER
tmux send -t kpinstall ./test_long.sh ENTER
tmux a -t kpinstall
