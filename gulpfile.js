

var gulp = require('gulp'),
  uglify = require('gulp-uglify'),
  concat = require('gulp-concat'),
  plumber = require('gulp-plumber'),
  sass = require('gulp-ruby-sass'),
  notify = require('gulp-notify'),
  runSequence = require('run-sequence');

var modulePath = 'modules/smartparticipation/smartparticipation_core/',
  moduleScriptsPath = modulePath + 'js/',
  librariesPath = 'libraries/',
  buildPath = modulePath + 'build/js',
  themePath = 'themes/',
  baseThemeStylesPath = themePath + 'smartparticipation_base/css/',
  smartparticipationDefaultThemeStylesPath = themePath + 'smartparticipation_default/css/';
  //smartparticipationDevThemeStylesPath = themePath + 'smartparticipation_dev/css/';

var paths = {
  scripts: [
    moduleScriptsPath + 'smartparticipation.js',
    moduleScriptsPath + 'breakpoint.js',
    moduleScriptsPath + 'subclass.js',
    moduleScriptsPath + '*.js'
  ],
  libraries: [
    librariesPath + 'infieldlabel/jquery.infieldlabel.min.js',
    librariesPath + 'jquery_easing/jquery.easing.1.3.js',
    librariesPath + 'jquery_ui/jquery-ui-1.11.4/jquery-ui.min.js',
    librariesPath + 'bootstrap-tooltip/tooltip.js',
    librariesPath + 'bootstrap-collapse/bootstrap-collapse.js',
    librariesPath + 'bootstrap-transition/bootstrap-transition.js',
    librariesPath + 'jquery_ui_carousel/jquery.rs.carousel.js',
    librariesPath + 'jquery_ui_multi_open_accordion/jquery.multi-accordion.js',
    librariesPath + 'jTruncate/jquery_truncator.js',
    librariesPath + 'liteaccordion/liteaccordion.jquery.min.js',
    librariesPath + 'modernizr/modernizr-2.6.2.min.js',
    librariesPath + 'mutate/mutate.js',
    librariesPath + 'ua_parser/ua-parser-0.7.9.min.js'
  ]
};

var onError = function (err) {
  console.log(err);
  notify({message: 'Error!'});
};

gulp.task('sp.libraries', function () {
  return gulp.src(paths.libraries)
    .pipe(plumber({errorHandler: onError}))
    .pipe(uglify({mangle: false}))
    .pipe(concat('smartparticipation.libraries.js'))
    .pipe(gulp.dest(buildPath))
});

gulp.task('sp.scripts', function () {
  return gulp.src(paths.scripts)
    .pipe(plumber({errorHandler: onError}))
    .pipe(uglify({mangle: false}))
    .pipe(concat('smartparticipation.scripts.js'))
    .pipe(gulp.dest(buildPath));
});

var buildStyle = function (sourceFileName, destPath) {
  return gulp.src(destPath + sourceFileName)
    .pipe(plumber({ errorHandler: onError }))
    .pipe(sass({ style: 'compressed', loadPath: baseThemeStylesPath, 'sourcemap=none': true }))
    .pipe(gulp.dest(destPath));
};

/*gulp.task('sp.styles.smartparticipation_dev', function() {
  return buildStyle('smartparticipation_Dev.scss', smartparticipationThemeStylesPath);
});*/

gulp.task('sp.styles.smartparticipation_default', function() {
  return buildStyle('smartparticipation_default.scss', smartparticipationDefaultThemeStylesPath);
});

gulp.task('sp.styles', function () {
  runSequence(
    'sp.styles.smartparticipation_default'
    // 'regroom.styles.smartparticipation_dev'
  );
});

gulp.task('watch', function () {
  gulp.watch(moduleScriptsPath + '*.js', ['sp.scripts'] );

  gulp.watch([
    baseThemeStylesPath + '**/*.scss',
    smartparticipationDefaultThemeStylesPath + '*.scss'
  ], ['sp.styles'] );

});
