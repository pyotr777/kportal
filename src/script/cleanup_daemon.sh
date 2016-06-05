echo "Find out all daemon from the login nodes"

if [ -d ~/.ssh/kportal ]; 
then
	for i in $(  ls -1 ~/.ssh/kportal/| sed -e 's/\..*$//' | uniq ); do
		echo "========$i======="
		ssh "$i.k.aics.riken.jp" 'rm -rf kportal/*' & echo " Clear!"
	done
else
	echo "folder is empty."
fi
echo "Done"
