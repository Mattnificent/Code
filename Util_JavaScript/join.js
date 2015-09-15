




function outer_join(a, aAccessor, aSelect, aPrefix, b, bAccessor, bSelect, bPrefix, keyLabel) {

    keyLabel = keyLabel || aAccessor;

    var aAccessorResult = checkAccessors(a, aAccessor);
    var bAccessorResult = checkAccessors(b, bAccessor);

    if(aAccessorResult.accessorsAreRedundant && bAccessorResult.accessorsAreRedundant) {
        throw "Both accessors can't be redundant!";
    }

    var full_set = _.union(aAccessorResult.accessorSet, bAccessorResult.accessorSet);


    
    if(bAccessorResult.accessorsAreRedundant) {
        console.log("B accessor was redundant!")
        return outer_join_redundancy_helper(full_set, a, aAccessor, aSelect, aPrefix, b, bAccessor, bSelect, bPrefix, keyLabel)
    } else if (aAccessorResult.accessorsAreRedundant){
        console.log("A accessor was redundant!")
        return outer_join_redundancy_helper(full_set, b, bAccessor, bSelect, bPrefix, a, aAccessor, aSelect, aPrefix, keyLabel)
    } else {
        console.log("both accessors were unique!")
        return outer_join_redundancy_helper(full_set, a, aAccessor, aSelect, aPrefix, b, bAccessor, bSelect, bPrefix, keyLabel)
    }



}

function outer_join_redundancy_helper( full_set, uc, ucAccessor, ucSelect, ucPrefix, rc, rcAccessor, rcSelect, rcPrefix, keyLabel) {

    var joinedCollection = [];

    _.each(full_set, function(entry, key) {

        console.log("-----------------------------------------------\nkey = " + key + ", entry = " + entry);

        // In this case the A accessor is unique
        // Let's find if there's an object in the collection where the aAccessor equals this particular id
        var uc_object = _.find(uc, function(uc_candidate) {
            return uc_candidate[ucAccessor] === entry;
        })

        // Now that we have
        var uc_object_new;
        if(uc_object) {
            var uc_object_key = {}, uc_object_remaining_fields = {};
            uc_object_key[keyLabel] = uc_object[ucAccessor];

            if(ucSelect == "*")
                uc_object_remaining_fields = _.omit(uc_object, ucAccessor);
            else {
                _.each(ucSelect, function(to_select) {
                    uc_object_remaining_fields[ucPrefix ? ucPrefix + "." + to_select : to_select] = uc_object[to_select];
                })
            }
            uc_object_new = _.assign(uc_object_key, uc_object_remaining_fields);
        }

        console.log("    uc_object = " + JSON.stringify(uc_object_new, null, 2));

        var match_collection = _.filter(rc, function(rc_candidate) {
            console.log("  rc_candidate = " + JSON.stringify(rc_candidate))
            return rc_candidate[rcAccessor] === entry;
        });

        var match_collection_new = [ ];

        _.each(match_collection, function(rc_object){

            console.log("   rc_object = " + JSON.stringify(rc_object));

            var rc_object_key = {}, rc_object_remaining_fields = {};

            if(uc_object_new)
                rc_object_key = uc_object_new;
            else
                rc_object_key[keyLabel] = rc_object[rcAccessor];

            if(rcSelect == "*")
                rc_object_remaining_fields = _.omit(rc_object, rcAccessor);
            else {
                _.each(rcSelect, function(to_select) {
                    console.log("   inside rc_object = " + JSON.stringify(rc_object) + ", " + JSON.stringify(match_collection_new));
                    rc_object_remaining_fields[rcPrefix ? rcPrefix + "." + to_select : to_select] = rc_object[to_select];

                })
            }

            console.log("     before match_collection_new = " + JSON.stringify(match_collection_new));
            match_collection_new.push(_.merge(_.cloneDeep(rc_object_key), _.cloneDeep(rc_object_remaining_fields)));
            console.log("     after match_collection_new = " + JSON.stringify(match_collection_new));
        });

        console.log("    match_collection = " + JSON.stringify(match_collection_new));
        console.log("    uc_object = " + JSON.stringify(uc_object_new));

        if(match_collection_new.length){
            joinedCollection = joinedCollection.concat(match_collection_new);
        } else if(uc_object_new != { }) {
            joinedCollection.push(uc_object_new)
        }

        console.log("    joinedCollection =====> " + JSON.stringify(joinedCollection));
    });

    return joinedCollection;
}


function checkAccessors(collection, accessorKey) {

    var accessorSet = [ ], accessorsAreRedundant = false;

    _.each(collection, function(value, key) {
        var accessorValue = value[accessorKey];
        if(accessorSet.indexOf(accessorValue) === -1)
            accessorSet.push(accessorValue);
        else accessorsAreRedundant = true;
    })

    return {
        accessorSet: accessorSet,
        accessorsAreRedundant: accessorsAreRedundant
    }
}






function outer_join_original(a, aAccessor, aSelect, aPrefix, b, bAccessor, bSelect, bPrefix, keyLabel) {

    keyLabel = keyLabel || aAccessor;

    var joinedCollection = [];

    var aAccessorResult = checkAccessors(a, aAccessor);
    var bAccessorResult = checkAccessors(b, bAccessor);

    if((!aAccessorResult.accessorsAreUnique) && (!bAccessorResult.accessorsAreUnique)) {
        throw "Both accessors can't be non-unique";
    }

    var full_set = _.union(aAccessorResult.accessorSet, bAccessorResult.accessorSet);

    if(!bAccessorResult.accessorsAreUnique) {


        _.each(full_set, function(entry, key) {

            console.log("-----------------------------------------------\nkey = " + key + ", entry = " + entry);

            var a_object = _(_.find(a, function(a_candidate) {
                    return a_candidate[aAccessor] === entry;
                }))
                .pick(function(key_candidate) {
                    return aSelect == "*" || _.includes(aSelect, key_candidate);
                })
                .mapValues(function(akey) {
                    return aPrefix ? aPrefix + "." + akey : akey;
                }).value();

            console.log("    a_object = " + JSON.stringify(a_object, null, 2));

            var match_collection = _(b)
                .filter(function(b_candidate) {
                    console.log("  b_candidate = " + JSON.stringify(b_candidate))
                    return b_candidate[bAccessor] === entry;
                })
                .map(function(original_b) {
                    console.log("  original_b = " + JSON.stringify(original_b))
                    return _(original_b)
                            .pick(function(key_candidate) {
                                //console.log("    key_candidate = " + key_candidate)
                                return bSelect == "*" || _.includes(bSelect, key_candidate);
                            })
                            .mapValues(function(bkey) {
                                //console.log("    bkey = " + bkey)
                                return bPrefix ? bPrefix + "." + bkey : bkey;
                            })
                            .value();
                })
                .map(function(b_fields_only){
                    return _.assign(b_fields_only, a_object);
                })
                .value();

            console.log("    match_collection = " + JSON.stringify(match_collection));
            console.log("    a_object = " + JSON.stringify(a_object));

            if(match_collection.length){
                joinedCollection = joinedCollection.concat(match_collection);
            } else if(a_object != { }) {
                joinedCollection.push(a_object)
            }

            console.log("    joinedCollection =====> " + JSON.stringify(joinedCollection));

        })

    } else if (!aAccessorResult.accessorsAreUnique){

        console.log("a accessor was not unique!")

    } else {
        console.log("both were unique!")

    }
    return joinedCollection;
}







///////////////////////////////////////////////////////////////////////////////////
// Test 1
///////////////////////////////////////////////////////////////////////////////////



var a = [
            {
                "count": 98,
                "to_status_label": "Closed Lost",
                "to_status_is_deleted": false
            },
            {
                "count": 39,
                "to_status_label": "Closed Won",
                "to_status_is_deleted": false
            },
            {
                "count": 666,
                "to_status_label": "Active",
                "to_status_is_deleted": true
            }
        ],
    b = [
            //{
            //    "gained": 14,
            //    "status_label": "Active",
            //    "change": 14,
            //    "left": 131
            //},
            //{
            //    "gained": 33,
            //    "status_label": "Active",
            //    "change": 54,
            //    "left": 11
            //},
            //{
            //    "gained": 39,
            //    "status_label": "Closed Won",
            //    "change": 39,
            //    "left": 1
            //}
        ];


//var buf = parseObjectToSpreadsheetBuffer(a);
//
//console.log(JSON.stringify(buf))
//
//google.updateWorksheet([ [], [], [] ], "CloseioReport", "test")



//var p = outer_join(a, "to_status_label", [ "count" ], "a", b, "status_label", [ "left",  "gained" ], "b", "status")
//var p = outer_join(b, "status_label", [ "gained", "left" ], "b",  a, "to_status_label", [ "count" ], "a", "status")

//console.log("---------------------------------------------------------\nResult:\n\n" + JSON.stringify(p, null, 2))


//google.updateWorksheet(parseObjectToSpreadsheetBuffer(p), "CloseioReport", "test")











///////////////////////////////////////////////////////////////////////////////////
// Test 2
///////////////////////////////////////////////////////////////////////////////////

//google.updateWorksheet([ [ 1, 2, 3 ], [ "a", "b", "weener" ] ], "CloseioReport", "status_transitions_summary")



var period = getPeriod(1);

var params = {
    user_id:"user_nvFb2ZwZRsA4u5YCdtzJ9IhXr5H4g1AKuAk6ljqXgsg",
    date_start: period.start,
    date_end: period.end
}


CloseIOReportToSpreadsheet(params);

function CloseIOReportToSpreadsheet(query) {
    closeio.getReportStatusesOpportunity(query).
        then(
        function(result) {

            var paramPrint = [];
            _.each(query, function(value, key) {
                paramPrint.push([ key, value ])
            });

            try{
                var joined_table = outer_join(  result["status_transitions_summary"],
                    "to_status_label",
                    [
                        "count",
                        "to_status_is_deleted",
                        "_query"
                    ],
                    "",
                    result["status_overview"],
                    "status_label",
                    [
                        "started",
                        "ended",
                        "change",
                        "change_percent",
                        "gained",
                        "lost",
                        "entered",
                        "left"
                    ],
                    "",
                    "status");

                google.updateWorksheet(paramPrint.concat(parseObjectToSpreadsheetBuffer(joined_table)), "CloseioReport", "Status Report")

            }catch(err) { console.log("Error " + err)}


            try{
                var sorted_table = outer_join(  result["status_transitions"],
                    "to_status_label",
                    [
                        "to_status_is_deleted",
                        "from_status_label",
                        "from_status_is_deleted",
                        "count",
                        "_query"
                    ],
                    "",
                    [],
                    "",
                    "*",
                    "",
                    "status");

                google.updateWorksheet(paramPrint.concat(parseObjectToSpreadsheetBuffer(sorted_table)), "CloseioReport", "Status Transition Report")

            }catch(err) { console.log("Error " + err)}

            _.each(result, function(report, name) {
                google.updateWorksheet(paramPrint.concat(parseObjectToSpreadsheetBuffer(report)), "CloseioReport", name)
            })

        }

    )

}





