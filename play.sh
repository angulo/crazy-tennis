if [ -f "code/CrazyTennis" ]
then
	cd config; ../code/CrazyTennis;
else
	echo "Project not built, first run:"
	echo "$ sh ./autogen.sh"
	echo "$ make"
fi
