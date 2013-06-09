AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-I "./include" \
	$(OGF_CFLAGS) $(OgreBullet_CFLAGS) $(bullet_CFLAGS) $(jsoncpp_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

AM_LDFLAGS = -I /usr/local/lib $(OGF_LIBS) $(OgreBullet_LIBS) $(bullet_LIBS) $(jsoncpp_LIBS)

bin_PROGRAMS = CrazyTennis

CrazyTennis_SOURCES = \
	src/main.cpp \
	src/InputAdapter.cpp \
	src/Model.cpp \
	src/SceneFactory.cpp \
	src/data/Loader.cpp \
	src/data/Match.cpp \
	src/data/Player.cpp \
	src/data/Tournament.cpp \
	src/data/TournamentFactory.cpp \
	src/dynamics/ShotSimulator.cpp \
	src/scenes/Match.cpp \
	src/scenes/MenuMain.cpp \
	src/scenes/Splash.cpp \
	src/widgets/Ball.cpp \
	src/widgets/PhysicalBase.cpp \
	src/widgets/PlayerBase.cpp

CrazyTennis_LDFLAGS = $(OGF_LIBS) $(OgreBullet_LIBS) $(bullet_LIBS) $(jsoncpp_LIBS)
CrazyTennis_LDADD = $(OGF_LIBS) $(OgreBullet_LIBS) $(bullet_LIBS) $(jsoncpp_LIBS)
