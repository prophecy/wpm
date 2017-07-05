# wpm (Wonder Package Manager)

### wpm is very personal package manager for general purpose. wpm make developer be possible to download packages (Following wonderconf.json) and run package script (Following conf.json). For example developer can inject any file from package to one's project.

## Structure
### Executable files
wpm  
wpmPackageInit

### Project structure and configurations
wonderconf.json  
wonder_modules/  
---- [module_name]/  
------ conf.json

### Sample
wpm install -clean  
wpm run wonderSample