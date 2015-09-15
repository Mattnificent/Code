
var S = require('string'),
    google = require('../data/google'),
    obj = {
    A: [
        {
            x:"this",
            y: {
                tits:2,
                tight:"yes"
            },
            z:"something else"
        },
        {
            x: { },
            y: "aloha",
            z: "earth"
        }
    ] ,
    B: "p",
    C: {
        a: { p:"hello" },
        b: "world"
    }

    },
    maxDepth = 10,
    buffer = [ ],
    _ = require('lodash');




//_.each(obj, test);
//
//function test(value, index) { console.log("INDEX = " + index + ", VALUE = " + JSON.stringify(value)) }


//console.log(_.keys(obj.A).length)

//console.log(getColumnSet_recursive(obj.A, 1))



// gets the column set for a collection's elements' subfields,
// for tabular comparison
function getColumnSet_recursive(collection, collection_depth){

    var columnSet = [], primitiveValueInCollection = false;

    _.each(collection, getThemKidsKeys);

    function getThemKidsKeys(kid_x) {

        console.log("kid_x = " + JSON.stringify(kid_x));

        (function recurse(master_value, path, depth){

            console.log("recursing: depth = " +  S(depth).pad(2).s +
                        ", path = " +  S(path).pad(4).s +
                        ", offspring = " + JSON.stringify(master_value));

            if( (typeof(master_value) === 'object')) {

                _.each(master_value, function (value, key) {

                    var columnName;

                    if (path !== "") columnName = _.isArray(master_value) ? path + "[" + key + "]" : path + "." + key;

                    else columnName = key;

                    if ((typeof(value) === 'object') && _.keys(value).length && (depth < maxDepth)) {
                        console.log("\t\t\t\t\t\t\t\t\t\tcolumnName = " + columnName + ", going deeper... ");
                        recurse(value, columnName, depth + 1)
                    } else if (columnSet.indexOf(columnName) === -1) {
                        console.log("\t\t\t\t\t\t\t\t\t\tcolumnName = " + columnName + ", adding column ");
                        columnSet.push(columnName);
                    }
                    else
                        console.log("\t\t\t\t\t\t\t\t\t\tcolumnName = " + columnName + ", nothing happening... ");
                });
            } else if ( path === "" && !primitiveValueInCollection) primitiveValueInCollection = true;


            console.log("\t\t\t\t\t\t\t\t\t\tpulling out...");

        })(kid_x, "", collection_depth)

    }

    return (primitiveValueInCollection ? ["VALUE"].concat(columnSet.sort()) : columnSet.sort())
}



//
// Checks children key-value pairs for matching grandchildren keys
//  - if there's a match (a common key), we want to get the column set
//
var checkForCommonKey = function(obj) {
    var keys = [], common_key = false;
    _.each(obj, function(value) {
        if(typeof(value) === 'object') {
            _.each(value, function(value, key) {
                if (_.contains(keys, key)) return true;
                keys.push(key);
            })
        }
    })
    return false;
}


var objects = [], paths = [];

function checkForCircularReferences(_object, path) {

    for (var i = 0; i < objects.length; i++) {
        if (objects[i] === _object)
            return "$ref = " + paths[i];
    }

    if(i == objects.length) {
        objects.push(_object);
        paths.push(path);
    }
}

var row = 0;

valuePrint_recurse(obj, 0, "");
printBuffer();

google.checkAndUpdateFile("test", buffer, "mbeck@smartprocure.us", "spreadsheet");


function valuePrint_recurse(value, column, path) {

    buffer.push([]);

    try{
        switch(Object.prototype.toString.call(value)) {
            case "[object Object]":
            case "[object TCP]":
            case "[object HTTPParser]":
            case "[object Array]":
                var circularReference, count;

                if(circularReference = checkForCircularReferences(value))
                    buffer[row][column] = circularReference;

                else if((count = value.length || Object.keys(value).length) == 0)
                    buffer[row][column] = _.isArray(value) ? "[ ]" : "{ }";

                else
                {
                    buffer[row][column] = _.isArray(value) ? "[...]" : "{...}";

                    column++;


                    var _common_key = common_key || checkForCommonKey(value);

                    if(_common_key) {
                        buffer[row].push(getColumnSet_recursive(value, "", level))
                    } else {
                        _.each(value, function (_value, _key) {

                            row++;
                            buffer.push([]);

                            buffer[row][column] = _key;

                            console.log("at " + (_.isArray(value) || _.contains(_key, '-') ?
                                path + "[" + _key + "]" : path + "." + _key ))
                            printBuffer();

                            valuePrint_recurse(_value,
                                column + 1,
                                _.isArray(value) || _.contains(_key, '-') ?
                                path + "[" + _key + "]" : path + "." + _key)
                        });
                    }
                }
                break;
            case "[object String]":
                buffer[row][column] = value;
                //buffer[row][column] = "\"" + value.replace(/"/g, "\\\"") + "\"";
                break;
            case "[object Number]":
            case "[object Boolean]":
            case "[object Null]":
                //case "[object Date]":
                //case "[object Undefined]":
                buffer[row][column] = value;
                break;
            case "[object Function]":
                buffer[row][column] = "\"" + "Function: " +
                    value.toString().split(/\r\n|\r|\n/).length + " lines\"";  // value.toString().replace(/"/g, "\\\"")
                break;
            default:
                buffer[row][column] = Object.prototype.toString.call(value);
                break;
        }
    } catch(err) { console.log("there was a firefight! " + err)}

}



var list_old = function(thing, row, column) {

    buffer.push([]);


    if(typeof(thing) === 'object') {

        // prints either [ ], [...], { }, or {...} in the value cell
        if(_.isArray(thing)) buffer[row][column] = "[" + (thing.length ? "..." : " ") + "]";
        else buffer[row][column] = "{" + (_.keys(thing).length ? "..." : " ") + "}";

        var _common_key = common_key || checkForCommonKey(thing);

        if(_common_key) {
            buffer[row].push(getColumnSet_recursive(thing, "", level))
        } else {
            _.each(thing, function(_value, _key) {

                buffer[row][++column] = _key;
                recurse(_value, level+1, _common_key, _common_key ? row+1 : row, _common_key ? column : column+1);
            });
        }


    } else {
        buffer[row][column] = obj;
    }


}



var spread = function(){

}




var recurse = function(obj, level, common_key, row, column) {

    if(level < maxDepth) {

        buffer.push([])
        if(typeof(obj) === 'object') {

            if(_.isArray(obj))
                buffer[row][column] = "[" + (obj.length ? "..." : " ") + "]";
            else
                buffer[row][column] = "{" + (Object.keys(obj).length ? "..." : " ") + "}";

            var _returnValue = checkForCommonKey_and_checkForPrimitive(obj),
                _common_key = common_key || _returnValue[0],
                _primitive = _returnValue[1];

            //console.log(stringToPrint + _common_key + ", " + _has_primitive);

            if(_primitive)
                buffer[row][++column] = "VALUE";


            if(_common_key) {
                buffer[row].push(getColumnSet_recursive(obj, "", level))
            }

            _.each(obj, function(_value, _key) {

                buffer[row][++column] = _key;
                recurse(_value, level+1, _common_key, _common_key ? row+1 : row, _common_key ? column : column+1);
            });
        } else {
            buffer[row][column] = obj;
        }
    }
}






//recurse(obj, 0, false, 0, 0);


function printBuffer() {

    var i, j;

    console.log("-------------------------------------------------------------------------------")

    try{
        var stringToPrint = "";
        for(i = 0; i<buffer.length; i++)
        {
            for(j = 0; j<buffer[i].length; j++)
                //console.log("i = " + i + ", j = " + j);
                //console.log(buffer[i][j]);
                stringToPrint += S(buffer[i][j] == undefined ? "" : buffer[i][j]).pad(10).s;
            stringToPrint += "\n";
        }


    } catch(err) { console.log("ERROR: " + err) }

    console.log(stringToPrint);

    console.log("-------------------------------------------------------------------------------")
}




