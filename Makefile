
default: all
##############################################################
include submodules/c_deps/etc/includes.mk

##############################################################
##############################################################
TIDIED_FILES = \
			   colors-csv-parser/*.c colors-csv-parser/*.h \
			   rgb-ansi-utils/*.c rgb-ansi-utils/*.h \
			   colors-json-parser/*.c colors-json-parser/*.h \
			   db/*.h db/*.c \
			   parse-colors/*.c parse-colors/*.h \
			   colors-test/*.c colors-test/*.h \
			   colors/*.c colors/*.h
##############################################################
do-write-etc-color-names:
	@./build/parse-colors/parse-colors -c 50000 -o ./etc/parsed-colors.json
##############################################################
