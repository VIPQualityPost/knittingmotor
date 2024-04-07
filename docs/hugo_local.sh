mkdir themes
cd themes
git clone https://github.com/thingsym/hugo-theme-techdoc techdoc
git clone https://github.com/mfg92/hugo-shortcode-gallery.git
cd ..
cp -a theme_mod/. themes/techdoc/

hugo serve --noHTTPCache --disableFastRender --cleanDestinationDir
rm -rf public
rm -rf themes
rm -rf resources
rm -f .hugo_build.lock

