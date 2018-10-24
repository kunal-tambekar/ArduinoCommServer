// Connecting to the Mongo Database
var mongo = require('mongodb');
var monk = require('monk');
var constants = require('./constants');

// <MODIFY>
var db = monk(constants.mongodburl);

/* SENSOR */

// ALSO use for update
exports.addNewSensor = function (ssr, succ, fail) {
    var collection = db.get('sensor_collection');
    collection.update({model_type:ssr.model_type},ssr, {upsert:true}, function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

// NOT USING THIS - USING add with upsert to modify existing sensor
exports.updateSensorWithType= function (id,sensor, succ, fail) {
    var collection = db.get('sensor_collection');
    collection.update({model_type:id},{$set:sensor}, function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

exports.removeSensorByType= function (t, succ, fail) {
    var collection = db.get('sensor_collection');
    collection.remove({model_type:t}, function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

/* sort
    0 - sort by name ascending ; 
    1 sort by name desc ; 
    2 sort by model_type asc ; 
    3 sort by model_type desc 
*/
exports.getAllSensors = function (sort,succ, fail) {
    var collection = db.get('sensor_collection');
    let filter = {sort: {name: 1}};
    switch(sort){
        case 1: filter = {sort: {name: -1}};
            break;
        case 2: filter = {sort: {model_type: 1}};
            break;
        case 3: 
            filter = {sort: {model_type: -1}};
        default:
            break;
    }

    collection.find({}, filter, function (err, docs) {
        if (err === null) {
            succ(docs);
        } else {
            fail(err);
        }
    });
}

exports.getSensorById = function (id, succ, fail) {

    var collection = db.get('sensor_collection');
    collection.findOne({
        model_type: id
    }, {}, function (err, doc) {
        if (err === null) {
            succ(doc);
        } else {
            fail(err);
        }
    });
}


/* ESP */
// USE FOR ADD NEW and UPDATE existing Esp - using upsert : Update and/or insert
exports.upsertEsp = function (esp, succ, fail) {
    var collection = db.get('esp_collection');
    esp.status = -1;
    // collection.insert(esp, function (err, result) 
    collection.update({ mac : esp.mac },esp, {upsert:true}, function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

exports.removeEspWithMac= function (del, succ, fail) {
    // 'del' can be '_id' or 'mac' : Shall be constructed in API wrapper
    var collection = db.get('esp_collection');
    collection.remove(del, function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

exports.updateEspWithMac= function (mac,esp, succ, fail) {
    var collection = db.get('esp_collection');
    collection.update({mac:mac},{$set:esp}, function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

exports.getAllEsps = function (filter, succ, fail) {
    var collection = db.get('esp_collection');
    let fltr;
    if (filter === null||filter===undefined) {
        fltr = {};
    } else {
        // 0 uncongifured ; 1 online ; 2 offline
        fltr ={status:filter};
    }
    collection.find(fltr, {sort: {status: 1,name: 1}}, function (err, docs) {
        if (err === null) {
            succ(docs);
        } else {
            fail(err);
        }
    });
}

exports.getEspByMac = function (mac, succ, fail) {

    var collection = db.get('esp_collection');
    collection.findOne({
        mac: mac
    }, {}, function (err, doc) {
        if (err === null) {
            succ(doc);
        } else {
            fail(err);
        }
    });
}

exports.getEspCount = function (succ, fail) {

    var collection = db.get('esp_collection');
    var count ={};
    collection.aggregate([
        {"$group" : {_id:"$status", count:{$sum:1}}}
    ], function (err, doc) {
        if (err === null) {
            succ(doc);
        } else {
            fail(err);
        }
    });
}

/* CONFIGURATION  */

exports.addNewConfiguration = function (obj,succ, fail) {

    var collection = db.get('configuration_collection');
    collection.insert(obj, function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

exports.removeConfigurationById = function (id,succ, fail) {

    var collection = db.get('configuration_collection');
    collection.remove({id:id}, function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

exports.updateConfigurationById = function (id,conf,succ, fail) {

    var collection = db.get('configuration_collection');
    collection.update({id:id}, {$set:conf},function (err, result) {
        if (err === null) {
            succ(result);
        } else {
            fail(err);
        }
    });
}

exports.getConfigurationByEspId = function (eid, succ, fail) {

    var collection = db.get('configuration_collection');
    collection.findOne({
        esp_id: eid
    }, {}, function (err, doc) {
        if (err === null) {
            succ(doc);
        } else {
            fail(err);
        }
    });
}

