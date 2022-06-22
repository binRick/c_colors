
default: all
##############################################################
PASSH=$(shell command -v passh)
GIT=$(shell command -v git)
SED=$(shell command -v gsed||command -v sed)
NODEMON=$(shell command -v nodemon)
FZF=$(shell command -v fzf)
BLINE=$(shell command -v bline)
UNCRUSTIFY=$(shell command -v uncrustify)
PWD=$(shell command -v pwd)
FIND=$(shell command -v find)
##############################################################
DIR=$(shell $(PWD))
LOADER_DIR=$(DIR)/loader
EMBEDS_DIR=$(DIR)/embeds
VENDOR_DIR=$(DIR)/vendor
PROJECT_DIR=$(DIR)
MESON_DEPS_DIR=$(DIR)/meson/deps
VENDOR_DIR=$(DIR)/vendor
DEPS_DIR=$(DIR)/deps
ETC_DIR=$(DIR)/etc
DOCKER_DIR=$(DIR)/docker
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
##############################################################
all: ensure dirs do-build do-test
do-write-etc-color-names:
	@./build/parse-colors/parse-colors -c 50000 -o ./etc/parsed-colors.json
do-meson:
	@eval cd . && {  meson build || { meson build --reconfigure || { meson build --wipe; } && meson build; }; } 
rm-make-logs:
	@rm .make-log* 2>/dev/null||true
test: do-test
do-build: do-meson
	@meson compile -C build
do-test:
	@passh meson test -C build -v --print-errorlogs
clean: 
	@rm -rf $(EMBEDS_DIR) build
	@rm -rf *.png
ensure: dirs-embeds
dirs: ensure dirs-embeds 
dirs-embeds:
	@mkdir -p $(EMBEDS_DIR)
uncrustify:
	@$(UNCRUSTIFY) -c submodules/meson_deps/etc/uncrustify.cfg --replace $(TIDIED_FILES) 
uncrustify-clean:
	@find  . -type f -name "*unc-back*"|xargs -I % unlink %
fix-dbg:
	@$(SED) 's|, % s);|, %s);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % lu);|, %lu);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % d);|, %d);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % zu);|, %zu);|g' -i $(TIDIED_FILES)
tidy: uncrustify uncrustify-clean fix-dbg
dev-all: all
pull:
	@git pull
dev-clean: clean dev
dev: pull tidy nodemon
nodemon: tidy
	@$(PASSH) -L .nodemon.log $(NODEMON) \
		--delay .1 \
		-w '*/meson.build' -w "*/*.c" -w "*/*.h" \
		-w 'include/*.h' -w meson.build -w src -w Makefile -w "loader/*" \
		-i build -i '*unc-back*' -i '*/subprojects' -i '*/embeds/*' \
		-e tpl,build,sh,c,h,Makefile \
		-x \
			env -- bash -c 'make dev-all||true'
git-pull:
	@git pull --recurse-submodules
git-submodules-pull-master:
	@git submodule foreach git pull origin master --jobs=10
git-submodules-update:
	@git submodule update --init	
meson-binaries:
	@meson introspect --targets  meson.build -i | jq 'map(select(.type == "executable").filename)|flatten|join("\n")' -Mrc

