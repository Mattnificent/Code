var google = require('../data/google'),
    Q = require('q'),
    _ = require('underscore'),
    util = require('../util'),
    fs = require('fs'),
    buffer;

module.exports = function(RED) {

    var nodeServerDestinationDirectory = "dump/";
    var node,
        fileName,
        location,
        shareWith,
        fileType,
        complete,
        specific,
        includeObjectKeyColumn,
        nameWithExtension,
        input;

    function toFileNode(config) {

        RED.nodes.createNode(this,config);

        this.on('input', function(msg) {

            node                    = this;
            fileName                = config.fileName;
            location                = config.location;
            shareWith               = config.shareWith;
            fileType                = config.fileType;
            complete                = config.complete;
            specific                = config.specific;
            includeObjectKeyColumn  = config.includeObjectKeyColumn;

            console.log("complete = " + complete);

            console.log("specific = " + specific);

            if(complete) input = msg;
            else input = msg[specific];


            nameWithExtension = config.fileName +
                (fileName.indexOf(".") != -1 ? "" :
                        (fileType === "spreadsheet" ? ( location === "nodeServer" ? ".csv" : "" ) :
                                (typeof input == "object" ? ".json" : ".txt")
                        )
                );

            console.log(nameWithExtension)
            if(fileType === "text") buffer = util.prettyPrint(input);
            else if(fileType === "spreadsheet") {

                switch (typeof ( input )) {
                    case 'object':
                        parseObjectToSpreadsheetBuffer(input, includeObjectKeyColumn);
                        break;
                    default:
                        node.warn(
                            "Error: can only parse an object or an array into a spreadsheet. You passed a(n) " +
                            typeof ( input ) +
                            " to the " + out + " property."
                        );
                        return;
                }
            }

            writeFileGeneric()

        });
    }

    function writeFileGeneric() {
        if(location === "nodeServer")         return writeToNodeServer();
        else if(location === "googleDrive")   return writeToGoogleDrive();
    }

    function writeToNodeServer() {
        console.log("Writing to Node Server...");

        return Q.ninvoke(fs, 'writeFile', nodeServerDestinationDirectory + nameWithExtension, buffer).
            then(
            function() {
                console.log(nodeServerDestinationDirectory + nameWithExtension + " file successfully written!");
            }).catch( function(err) {
                console.log("    Error writing file: " + err)
            } );
    }

    function writeToGoogleDrive() {
        console.log("Writing to Google Drive");
        google.checkAndUpdateFile(nameWithExtension, buffer, shareWith, fileType);
    }


    function parseObjectToSpreadsheetBuffer(obj, includeObjectKeyColumn_bool){

        var columnSet = [];

        // Create the exclusive set of columns (from the object keys)
        _.each(obj,function(value) {
            _.each(_.keys(value), function(subKey) {
                if(columnSet.indexOf(subKey) === -1)
                    columnSet.push(subKey);
            });
        });

        // Add the "object_key" column if the option is selected
        var firstRow = includeObjectKeyColumn_bool ? ["object_key"].concat(columnSet) : columnSet;

        // Add the first row; the column headers
        if(location === "nodeServer")       buffer = firstRow.join();
        else if(location === "googleDrive") buffer = [ firstRow ];

        var helperArray;

        // For each of the items in the array/object...
        _.each(obj,function(value, key) {

            helperArray = [];

            // Add the "object_key" column if the option is selected
            if(includeObjectKeyColumn_bool)  helperArray.push(key);

            // For each of the columns that were discovered earlier...
            _.each(columnSet,function(column) {

                if(_.isObject(value[column]))
                    helperArray.push(JSON.stringify(value[column]));

                // Blank the nulls and undefined's
                else if(_.isNull(value[column]))
                    helperArray.push("");
                else if(_.isUndefined(value[column]))
                    helperArray.push("");

                else
                    helperArray.push(String(value[column]));
            });

            if(location === "nodeServer")       buffer += "\n" + helperArray.join();
            else if(location === "googleDrive") buffer.push(helperArray);
        });

        helperArray = null;

    }




    RED.nodes.registerType("tofile", toFileNode);
};




