SUBDIRS = code

DIST_SUBDIRS = code

crazytennisdocdir = ${prefix}/doc/CrazyTennis

crazytennisdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = $(crazytennisdoc_DATA)

clean-local:
	cd media/models/; make clean;

all-local:
	cd media/models/; make;
