var fs = require('fs');

//Add type to the list if you find new one left side is what you find in json and right side is what you expect it to be in your kafka file
//Example "string": "STRING" will try to find "string" in json and "STRING" will be stored in the kafka query

const typelist = {
  "string": "STRING",
  "int": "INT",
  "double": "INT",
  "long": "STRING",
  "float": "DOUBLE",
  "boolean": "BOOLEAN",
  "double": "DOUBLE"
}

// Replace {} in the variables below with the whole Schema in the given JSON file Schema {"fields":...}

var json1= {}
var json2 = {}
var json3 = {}

// Add more json variable above and add the variables to the jsonlist list so that it gets processed along


jsonlist = [json1,json2]
function c2u(name) {
    name = name.replace(/([A-Z])/g, "_$1").toUpperCase()
    return name
}

function field2string(fields){
    var string1 = ""
    fields.forEach(element => {
      if (element.type.fields){
        data = field2string(element.type.fields)
        string1+= element.type.name + " STRUCT <" + data + ">"
      } else {
        string1+=element.name + " " + typelist[element.type] + ",\n"
      }
    });
    // string1 = string1.replace(/,\n([^,\n]*)$/, '$1')
    return string1
}
jsonlist.forEach(element => {
  var query1 = "CREATE STREAM " + element.namespace.split(".")[1].toUpperCase() + c2u(element.name) + "    ( \n"
  data = field2string(element.fields).replace(/,\n([^,\n]*)$/, '$1')
  data = data.replace(",\n>", ">")
  buffer = data.match(/>[A-Za-z]{1}/)
  if(buffer){
  console.log(buffer[0])
  data1 = data.split(/>[A-Za-z]{1}/)
  data = data1[0]+ buffer[0][0]+",\n" + buffer[0][1] + data1[1]
  }
var final_query = query1 + data + "\n ) WITH (kafka_topic='" + element.namespace.split(".")[1] + "." + element.name + "', partitions=" + (element.fields.length-1).toString() + ", value_format='JSON');\n\n\n";

// This will append the generated kafka query into the file So after every run you will need to move/delete the file as it will keep appending the data

fs.appendFile("./newdata.txt", final_query, function (err) {
  if (err) throw err;
  console.log('Saved!');
});
})
