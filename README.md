# ofxASR_example
Attempt at using example code from ofxASR addon for openFrameworks.

# installation
Clone https://github.com/ludimation/ofxASR_example.git to "<openFrameworks>\apps\myApps\".

# dependencies
ofxASR--https://github.com/ludimation/ofxasr.git
* Download / clone locally to disk
* Navigate to "ofxASR" subdirectory and copy it to "<openFrameworks>\addons\" directory.

# notes
When using the project generator in Win32 to create a new visual studio project with ofxASR, it doesn't seem to link the sphinx ".lib" and ".dll" files properly--results in some compile errors, as well as some missing "dll" errors at runtime
  * "link" and "export" portions of the install xml file don't seem to work--only "include" entries were added properly to the sln.
  * Not sure how to fix the xml file, so I hacked in the necessary dependencies:
    * Added sphinx ".lib" files by hand.
    * Created a 'dlls" directory at the root level of the project and added a post-build macro to copy them to "bin".

Model resources are required in order to use ofxASR
* I tested both both the <a href="https://code.google.com/p/ofxasr/downloads/detail?name=typical_model.tar.gz">typical model resource pack</a> and the <a href="https://code.google.com/p/ofxasr/downloads/detail?name=sphinxmodel_voxforge-en-r0_1_3.tar.gz">complete english model resource pack</a>.
* If you'd like to use others, see the FAQ on the addOn google code page--https://code.google.com/p/ofxasr/wiki/FAQ
