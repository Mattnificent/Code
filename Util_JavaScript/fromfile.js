var google = require('../data/google'),
    Q = require('q'),
    _ = require('underscore'),
    fs = require('fs'),
    buffer;

module.exports = function(RED) {

    var nodeServerDestinationDirectory = "dump/";
    var node,
        fileName,
        location,
        fileType,
        dataType;

    function fromFileNode(config) {

        node        = this;
        fileName    = config.fileName;
        location    = config.location;
        fileType    = config.fileType;
        dataType    = config.dataType;

        RED.nodes.createNode(this,config);

        this.on('input', function(msg) {

            readObjectFromFile(nodeServerDestinationDirectory + fileName).then(function (fileContents) {
                msg.payload = fileContents;
                node.send(msg);
            });

        });
    }

    function readObjectFromFile(fileName_string) {
        return Q.ninvoke(fs, 'readFile', fileName_string ).
            then(
            function(fileContents_string) {

                var variable;
                try { variable = JSON.parse(fileContents_string); }
                catch(err) { variable = fileContents_string }

                if(_.isArray(variable)) console.log("Successfully read array with " + variable.length + " elements.")
                else console.log("Successfully read " + typeof variable);

                return variable;
            });
    }


    RED.nodes.registerType("fromfile", fromFileNode);
};
