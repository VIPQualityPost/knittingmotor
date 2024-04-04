mkdir themes
cd themes
git clone https://github.com/thingsym/hugo-theme-techdoc techdoc
cd ..
hugo serve --noHTTPCache --disableFastRender --cleanDestinationDir
rm -rf public
rm -rf themes
rm -f .hugo_build.lock

