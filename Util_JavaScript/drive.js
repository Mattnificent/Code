var googleSpreadsheet = require('google-spreadsheet'),
    editGoogleSpreadsheet = require('edit-google-spreadsheet'),
    globalConfig = require('../config'),
    util = require('../util'),
    _ = require('underscore'),
    Q = require('q'),
    S = require('string'),
    googleapis = require('googleapis'),
    nodeUtils = require('util'),
    loadSpreadsheet = Q.denodeify(editGoogleSpreadsheet.load),
    jwt = new googleapis.auth.JWT(
        globalConfig.gAppsSettings.user,
        globalConfig.gAppsSettings.keyFileLocation,
        null,
        [
            'https://spreadsheets.google.com/feeds',
            'https://www.googleapis.com/auth/drive'
        ]);




exports.loadSheet = function(spreadsheetName, worksheetName) {
    return Q.ninvoke(editGoogleSpreadsheet, 'load', {
        spreadsheetName: spreadsheetName,
        worksheetName: worksheetName || spreadsheetName,
        oauth: {
            email: globalConfig.gAppsSettings.user,
            keyFile: globalConfig.gAppsSettings.keyFileLocation
        }
    })
}


exports.updateWorksheet = function(buffer, spreadsheetName, worksheetName) {
    return exports.updateSpreadsheetMetadata(buffer, spreadsheetName, worksheetName).
        then( exports.updateCells(buffer, spreadsheetName, worksheetName) ).
        catch( function(err) { console.log("    ERRROR! " + err); } )
}




var drive;

// Does the following:
//   Checks if file exists
//     (creates it if it doesn't)
//   Checks if file is shared with email
//     (shares it if it is not already)
//   Updates the spreadsheet's height and width
//   Updates the spreadsheet's cell contents
exports.checkAndUpdateFile = function(fileName, buffer, shareWith, fileType) {

    drive = googleapis.drive({ version: 'v2', auth: jwt });

    var fileID;

    return exports.getFileID(fileName).
            then(
                function(_fileID) {
                    if(!_fileID) {
                        console.log("    " + fileName + ", did not exist.");
                        return exports.createFile(fileName, fileType);
                    }
                    else
                    {
                        console.log("    " + fileName +
                            ", already exists. (fileID = " + _fileID + ")");
                        return _fileID;
                    }
                }).
            then( function(fileid) {
                fileID = fileid;
                return exports.shareFile(fileid, shareWith, fileName);
            }).
            then(
                function() {
                    if(fileType == "spreadsheet")
                        return exports.updateSpreadsheetMetadata(buffer, fileName);
                    else
                        return exports.updateFile(fileID, buffer);
            }).
            then(
                function() {
                    if(fileType == "spreadsheet")
                        return exports.updateCells(buffer, fileName);
            }).
            catch( function(err) { console.log("ERROR: " + err) });
};


// Returns the fileID of the fileName, if the file is found
// Returns null otherwise
exports.getFileID = function(fileName){

    console.log("Checking if " + fileName + " exists...");

    return Q.ninvoke(drive.files, 'list', {
        maxResults: 1,
        q: "title=\'"+fileName+"\'",
        auth: jwt
    }).spread( function(res) {
        return res.items.length == 0 ? null : res.items[0].id;
    });

};

// Creates a spreadsheet by the name of fileName
// Returns the fileID
exports.createFile = function(fileName, fileType){

    console.log("    Creating " + fileType + " file; " + fileName + "...");

    return Q.ninvoke(drive.files, 'insert', {
        resource: {
            title: fileName,
            mimeType:  fileType == "spreadsheet" ? 'application/vnd.google-apps.spreadsheet' : "PLAIN_TEXT"
        },
        media: {
            mimeType: fileType == "spreadsheet" ? 'text/csv' : 'text/plain',
            body: fileType + " file created!"
        }
    }).spread(
        function(res) {
            console.log("    " + fileName + " created! (fileID = " + res.id + ")");
            return res.id
        }
    );
};


// Updates a file by the name of fileName with the contents of buffer
// Returns the fileID
exports.updateFile = function(fileID, buffer){

    console.log("Updating " + fileID + " with buffer " + buffer + "...");

    return Q.ninvoke(drive.files, 'update', {
            fileId: fileID,
            media: {
                mimeType: 'text/plain',
                body: JSON.stringify(buffer, null, 2)
            }
    }).spread(
        function(res) {
            console.log("    " + fileID + " FILE UPDATED! (fileID = " + res.id + ")");
            return res.id
        }
    );
};

// Checks if the email has explicit access to the file
// Shares file with email, if the permission wasn't already there
// Returns nonsense... don't trust return values, unless you overhaul the method, and regression test it!
exports.shareFile = function(fileID, email, spreadsheetName){

    console.log("Checking if " + spreadsheetName + " is shared with " + email);

    return Q.ninvoke(drive.permissions, 'list', {
        fileId: fileID
    }).spread(
        function(res) {
            if(_.find(res.items, function(permission) { return permission.emailAddress == email  }) == null)
            {
                console.log("    " + spreadsheetName + " was not shared with " + email + ". Sharing now..." );
                return Q.ninvoke(drive.permissions, 'insert', {
                    fileId: fileID,
                    resource: {
                        value: email,
                        type: 'user',
                        role: 'writer'
                    }
                });
            }
            console.log("    " + spreadsheetName + " was ALREADY SHARED with " + email + ". " );
        }).then(
            function(res) {
                if(res)
                    console.log("    " + spreadsheetName + " SUCCESSFULLY SHARED with " + email + "!" );
                return fileID;
            }
    );
};


// Loads the spreadsheet, and updates the number of rows and columns in the spreadsheet
// Returns nothing
exports.updateSpreadsheetMetadata = function(buffer, spreadsheetName, worksheetName){

    console.log("Loading " + spreadsheetName + " to update metadata...");

    return exports.loadSheet(spreadsheetName, worksheetName).
        then(
        function(spreadsheet) {
            console.log("    updating metadata...");

            var max_width = 1;
            _.each(buffer, function(array){
                if(array.length > max_width) max_width = array.length;
            })

            return Q.ninvoke(spreadsheet, 'metadata', {
                title: worksheetName || spreadsheetName,
                rowCount: buffer.length,
                colCount: max_width
            });
        }).then(
        function() {
            console.log("    METADATA UPDATED!");
        }).catch( function(err) {
            console.log("ERRRR: " + err)
        });
};


// Loads the spreadsheet, and updates the actual cells in the spreadsheet
// Returns nothing
exports.updateCells = function(buffer, spreadsheetName, worksheetName){

    console.log("Loading " + spreadsheetName + " to update cells...");

    return exports.loadSheet(spreadsheetName, worksheetName).
        then(
        function(spreadsheet) {
            console.log("    updating cells...");
            spreadsheet.add(buffer);
            return Q.ninvoke(spreadsheet, 'send');
        }
    ).then(
        function() {
            console.log("    CELLS UPDATED!");
        }
    );
};



