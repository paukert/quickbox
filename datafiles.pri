unix {
	CONFIG(debug, debug|release) {
		QMAKE_CLEAN += $$DEST_DATA_DIR/datafiles_installed
		POST_TARGETDEPS += $$DEST_DATA_DIR/datafiles_installed
		QMAKE_EXTRA_TARGETS += $$DEST_DATA_DIR/datafiles_installed
		$$DEST_DATA_DIR/datafiles_installed.commands = \
			mkdir -p $$DEST_DATA_DIR \
			&& ln -sf $$SRC_DATA_DIR/* $$DEST_DATA_DIR \
			&& touch $$DEST_DATA_DIR/datafiles_installed
	}
	else {
		POST_TARGETDEPS += datafiles
		QMAKE_EXTRA_TARGETS += datafiles
		datafiles.commands = \
			mkdir -p $$DEST_DATA_DIR \
			&& rsync -r $$SRC_DATA_DIR/ $$DEST_DATA_DIR
	}
}
win32 {
	POST_TARGETDEPS += datafiles
	QMAKE_EXTRA_TARGETS += datafiles

	datafiles.commands = \
		xcopy $$shell_quote($$shell_path($$SRC_DATA_DIR)) $$shell_quote($$shell_path($$DEST_DATA_DIR)) /s /e /y /i
		#xcopy \"$$shell_path($$SRC_DATA_DIR)\" \"$$shell_path($$DEST_DATA_DIR)\" /s /e /y /i
		#robocopy $$shell_path($$SRC_DATA_DIR) $$shell_path($$DEST_DATA_DIR) /IS /E
		# robocopy 0-7 exit codes are not an error
		#IF %ERRORLEVEL% LSS 8 SET ERRORLEVEL=0
}
