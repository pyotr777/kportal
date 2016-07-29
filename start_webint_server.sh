#!/bin/bash
# Start webint for installing K-portal.

echo -en "\033[38;5;70mStarting installation server in tmux session kpinstall \033[m\n" > usage.txt
echo -en "\033[38;5;70mTo attach to tmux session run: \033[m\n" >> usage.txt
echo -en "\033[48;5;237mtmux a -t kpinstall \033[m\n" >> usage.txt
echo -en "\033[38;5;70mTo detach press keys: \033[m\n" >> usage.txt
echo -en "\033[48;5;237mCtrl+b d \033[m\n" >> usage.txt
tmux new -s kpinstall -d
tmux send -t kpinstall "cat \$HOME/usage.txt" ENTER
tmux send -t kpinstall "cd \$HOME/kportal/webint" ENTER
tmux send -t kpinstall ./webint.py ENTER
tmux a -t kpinstall



