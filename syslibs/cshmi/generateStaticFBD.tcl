# Tcl Script to generate an FBD template file from HTTP files
#
# Usage:
#   generateStaticFBD.tcl STATIC_DIR OUTPUT_FILE
#
# where STATIC_DIR is the path of the staticfiles directory and OUTPUT_FILE is the .fbd file to generated (typicall
# "StaticDisplayComponent.fbd")

puts "== Begin processing js files ==" 
cd [lindex $argv 0]

set contentname "content"
set mimetypename "mimetype"
set encodingname "encoding"
set webserverlib "kshttp"
set staticfileclassname "staticfile"
set basehttppath "/hmi"
set baseovpath "/data/${webserverlib}${basehttppath}"
set staticfileclass "/acplt/${webserverlib}/${staticfileclassname}"
set fbdfilename [lindex $argv 1]

proc stripLineComments {inputString {commentChars ";#"}} {
	# Switch the RE engine into line-respecting mode instead of the default whole-string mode
	regsub -all -line "(\s+)\/\/.*$" $inputString "" commentStripped
	# Now strip the whitespace
	regsub -all -line {^[ \t\r]*(.*\S)?[ \t\r]*$} $commentStripped {\1}
}

proc printDomain {ovpath} {
	global out
	
	puts $out " INSTANCE  $ovpath :"
	puts $out "    CLASS /acplt/ov/domain;"
	puts $out " END_INSTANCE;"
	puts $out ""
}

proc processDir {ovpath} {
	global out
	global contentname mimetypename encodingname webserverlib staticfileclass
	
	set filelist [glob -nocomplain -types {f} *]
	foreach filename $filelist {
		puts -nonewline "Including file: $filename..."
		#reading file
		set in [open $filename r]
		#mimetype business
		set extension [file extension $filename]
		
		#default value
		set mimetype "text/html"
		set encodingvalue "windows-1252"
		if { $extension == ".tcl" } {
			puts "skipping myself"
			continue
		} elseif { $filename == ".project" || $filename == ".settings" || $filename == ".svn"} {
			puts "skipping generated file"
			continue
		} elseif { $extension == ".fbd" } {
			puts "skipping generated file"
			continue
		} elseif { $extension == ".exe" } {
			puts "skipping exe binary file"
			continue
		} elseif { $extension == ".xhtml" } {
			set mimetype "application/xhtml+xml"
		} elseif { $extension == ".xml" } {
			set mimetype "text/xml"
		} elseif { $extension == ".js" } {
			set mimetype "text/javascript"
		} elseif { $extension == ".css" } {
			set mimetype "text/css"
		} elseif { $extension == ".svg" } {
			set mimetype "image/svg+xml"
		} elseif { $extension == ".jpg" || $extension == ".jpeg" || $extension == ".png" || $extension == ".ico" || $extension == ".gif" || $extension == ".log" || $extension == ".php"} {
			#ignore binary formats
			puts "skipping binary or unwanted file"
			continue
		} elseif { $extension == ".bak" } {
			puts "skipping backup file"
			continue
		}
		
		set percentfilename [string map {"." "%2E"} $filename]
		set percentfilename [string map {"-" "%2D"} $percentfilename]
		puts $out " INSTANCE  ${ovpath}/${percentfilename} :"
		puts $out "    CLASS ${staticfileclass};"
		puts $out "    VARIABLE_VALUES"
		puts -nonewline $out "        ${contentname} : INPUT  STRING = \""
		
		# line-by-line, read the original file
		while {[gets $in line] != -1} {
			#transform $line somehow, order of manipulation matters
			
#			#recode backslashes
#			set line [string map {"\\" "\\\\"} $line]
			#recode quotes
			set line [string map {"\"" "\\\""} $line]
			if { $extension == ".js" || $extension == ".html" || $extension == ".xhtml"} {
				#strip line comments and trim white spaces at line begin and end
#this is buggy, so deactivate
#				set line [stripLineComments $line]
			}
			#recode lineending
#			set line "$line\\n\\"
			# then write the transformed line
			puts $out $line
		}
		close $in
		puts $out "\";"
		puts $out "        ${mimetypename} : INPUT  STRING = \"$mimetype\";"
		puts $out "        ${encodingname} : INPUT  STRING = \"$encodingvalue\";"
		puts $out "    END_VARIABLE_VALUES;"
		puts $out " END_INSTANCE;"
		puts $out ""
		puts "done"
	}
	
	#Search for child directories
	set dirlist [glob -nocomplain -types {d} *]
	foreach dirname $dirlist {
		if { $dirname == "tclsh.exe"} {
			puts "skipping false positive directory: $dirname"
			continue
		} elseif { $dirname == ".settings" || $dirname == ".svn" || $dirname == "bugList" || $dirname == "exportKSX" || $dirname == "requirementTests" || $dirname == "bak" || $dirname == ".externalToolBuilders"} {
			puts "skipping directory: $dirname"
			continue
		}
		cd $dirname;
		set percentdirname [string map {"." "%2E"} $dirname]
		set percentdirname [string map {"-" "%2D"} $percentdirname]
		set newovpath "$ovpath/$percentdirname"
		
		puts "Processing directory: $dirname"
		
		printDomain $newovpath
		processDir $newovpath
		
		cd ".."
	}
}

puts -nonewline "Building FBD header..."
set out [open ${fbdfilename} w]
puts $out "/******************************************************************************************"
puts $out "These are all static files of the display component of csHMI"
puts $out "which have to be loaded into an OV server."
puts $out "The webserver library ${webserverlib} will be loaded automatically."
puts $out ""
puts -nonewline $out "generated with the skript generateFBD.tcl: "
puts $out [clock format [clock seconds] -format {%Y-%m-%d %H:%M:%S}]
puts $out "Do not edit this file"
puts $out "******************************************************************************************/"
puts $out ""
puts "done"

printDomain $baseovpath
processDir $baseovpath

#fb_dbcommands is (2013-04-26, svn 7534) not able to handle a full library path, only LTsoft has a patched source of dbcommands
#puts $out ""
#puts $out " LIBRARY"
#puts $out "    /acplt/${webserverlib}"
#puts $out " END_LIBRARY;"

close $out
puts ""
puts "File $fbdfilename written."

puts "== End processing static files =="
