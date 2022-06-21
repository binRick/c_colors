for f in etc/colornames.bestof etc/colornames; do for m in csv json; do ./build/parse-colors/parse-colors -C $f.csv -J $f.json -D $f.sqlite -c 999999 -m $m; done; done
