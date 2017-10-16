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

### Command list (Shown when type wpm help)
+ exec: Run OS command  
  + -reverse_copy: Execute reverse copy  
+ help: Show helpful info  
+ install: Install package  
  + -clean: Remove repository before downloading it.  
+ npm: Call npm  
+ run: Execute command(s) inside the package  
+ uninstall: Uninstall package  
+ update: Update package  
+ version: Show wpm version  
+ view: View package info  