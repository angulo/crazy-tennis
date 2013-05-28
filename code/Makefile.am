AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-I "./include" \
	$(OGF_CFLAGS) $(OgreBullet_CFLAGS) $(bullet_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

AM_LDFLAGS = -I /usr/local/lib $(OGF_LIBS) $(OgreBullet_LIBS) $(bullet_LIBS)

bin_PROGRAMS = crazy-tennis

crazy-tennis_SOURCES = \
	src/main.cpp

crazy-tennis_LDFLAGS = $(OGF_LIBS) $(OgreBullet_LIBS) $(bullet_LIBS)
crazy-tennis_LDADD = $(OGF_LIBS) $(OgreBullet_LIBS) $(bullet_LIBS)
