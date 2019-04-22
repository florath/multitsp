/*
* Address to Longitute/Latitude conversion.
*
* This is loosly based on
* https://github.com/nuket/google-sheets-geocoding-macro/blob/master/Code.gs
*/

function convertOneAddressToCoordinates(geocoder, addressRow) {
  Logger.log("convertOneAddressToCoordinates", addressRow);
  if (addressRow.getCell(1, 5).isBlank() == false) {
    Logger.log("Latitude already set - skipping");
    return;
  }
  var address = addressRow.getDisplayValues().join(' ');
  address = address.replace(/'/g, "%27");
  
  Logger.log("Geocoding ", address);
  // Geocode the address and plug the lat, lng pair into the 
  // last 2 elements of the current range row.
  location = geocoder.geocode(address);
  
  // Only change cells if geocoder seems to have gotten a 
  // valid response.
  if (location.status == 'OK') {
    lat = location["results"][0]["geometry"]["location"]["lat"];
    lng = location["results"][0]["geometry"]["location"]["lng"];
    
    addressRow.getCell(1, 5).setValue(lat);
    addressRow.getCell(1, 6).setValue(lng);
  } else {
    Logger.log(location.status);
  }
};

function convertOneRangeToCoordinates(activeSheet, geocoder, range) {
  Logger.log("convertOneRangeToCoordinates", range.getRow(), range.getLastRow());
  
  for(var ridx = range.getRow(); ridx <= range.getLastRow(); ridx++) {
    convertOneAddressToCoordinates(
      geocoder, activeSheet.getRange(ridx, range.getColumn(), 1, 6));
  }
};

// Geocoding: convert addresses to GPS positions
function addressToCoordinates() {
  var activeSheet = SpreadsheetApp.getActiveSheet();
  var selection = activeSheet.getSelection();
  var geocoder = Maps.newGeocoder().setRegion("de");
  
  var ranges = selection.getActiveRangeList().getRanges();
  for (var range_idx = 0; range_idx < ranges.length; range_idx++) {
    convertOneRangeToCoordinates(activeSheet, geocoder, ranges[range_idx]);
  }
  return;
};

// ==========================================

function addOneAddressToMap(map, dataRow) {
  Logger.log("addOneAddressToMap", dataRow);
  //  var address = addressRow.getDisplayValues().join(' ');
  //  address = address.replace(/'/g, "%27");
  map.addMarker(dataRow.getCell(1, 5).getValue(),
                dataRow.getCell(1, 6).getValue())
};

function addOneRangeToMap(activeSheet, map, range) {
  Logger.log("addOneRangeToMap", range.getRow(), range.getLastRow());
  for(var ridx = range.getRow(); ridx <= range.getLastRow(); ridx++) {
    addOneAddressToMap(
      map, activeSheet.getRange(ridx, range.getColumn(), 1, 6));
  }
};

function sendMap() {
  var activeSheet = SpreadsheetApp.getActiveSheet();
  var selection = activeSheet.getSelection();
  var map = Maps.newStaticMap().setSize(1024, 1024).setZoom(11);

  var ranges = selection.getActiveRangeList().getRanges();
  for (var range_idx = 0; range_idx < ranges.length; range_idx++) {
    addOneRangeToMap(activeSheet, map, ranges[range_idx]);
  }
  GmailApp.sendEmail('andreas.florath@parzival.schule', 'Map', 'See below.', {attachments:[map]});
  return;
};

// =================================================

function copyColumToDistanceSheet(fromIdx, toIdx) {
  var activeSpreadsheet = SpreadsheetApp.getActiveSpreadsheet();
  var activeSheet = SpreadsheetApp.getActiveSheet();
  var distanceSheet = activeSpreadsheet.getSheetByName("Entfernungen");
  var range = activeSheet.getActiveRange();
  
  var source = activeSheet.getRange(range.getRow(), fromIdx, range.getLastRow());
  var destination = distanceSheet.getRange(2, toIdx, range.getLastRow() - range.getRow(), 1)
  source.copyTo(destination);
}

function computeNumberOfPlaces(distanceSheet, cnt) {
  Logger.log("computeNumberOfPlaces cnt ", cnt);
  var placesRange = distanceSheet.getRange(2, 4, cnt + 1, 1);
  // School is always 0 - everything
  placesRange.getCell(1, 1).setValue(0);
  for (var ridx = 2; ridx < cnt + 2; ridx++) {
    if (placesRange.getCell(ridx, 1).isBlank()) {
      placesRange.getCell(ridx, 1).setValue(1);
    } else {
      placesRange.getCell(ridx, 1).setValue(placesRange.getCell(ridx, 1).getValue() + 1);
    }
  }
}

function setEntryTimeToSecs(distanceSheet, rangeRowCnt, offset) {
  Logger.log("setEntryTimeToSecs cnt ", rangeRowCnt);
  var entryTimeRange = distanceSheet.getRange(2, offset, rangeRowCnt + 1, 1);
  for (var ridx = 1; ridx < rangeRowCnt + 2; ridx++) {
    entryTimeRange.getCell(ridx, 1).setValue(entryTimeRange.getCell(ridx, 1).getValue() * 60);
  }
};

function setColumnIndices(distanceSheet, rangeRowCnt, offset) {
  var indicesRange = distanceSheet.getRange(1, offset, 1, rangeRowCnt + offset);
  for (var cidx = 1; cidx < rangeRowCnt + 2; cidx++) {
    indicesRange.getCell(1, cidx).setValue(cidx - 1);
  }
};

function prepareDistanceSheet() {
  var activeSpreadsheet = SpreadsheetApp.getActiveSpreadsheet();
  var activeSheet = SpreadsheetApp.getActiveSheet();
  var distanceSheet = activeSpreadsheet.getSheetByName("Entfernungen");
  var range = activeSheet.getActiveRange();
  var rangeRowCnt = range.getLastRow() - range.getRow();

  distanceSheet.clear();
  
  // Write number of places in A1:
  distanceSheet.getRange(1, 1, 1, 1).setValue(rangeRowCnt)

  // Numbers
  copyColumToDistanceSheet(1, 1);
  // Additional Places
  copyColumToDistanceSheet(6, 4);
  // Entry Time
  copyColumToDistanceSheet(7, 5);
  // Latitude
  copyColumToDistanceSheet(12, 2);
  // Longitude
  copyColumToDistanceSheet(13, 3);
  
  distanceSheet.clearFormats();
  
  setEntryTimeToSecs(distanceSheet, rangeRowCnt, 5);
  setColumnIndices(distanceSheet, rangeRowCnt, 6);
  computeNumberOfPlaces(distanceSheet, rangeRowCnt);
 
  distanceSheet.autoResizeColumns(1, 6 + rangeRowCnt);
};

// =================================================

function lookForSamePlaces() {
  var activeSpreadsheet = SpreadsheetApp.getActiveSpreadsheet();
  var distanceSheet = activeSpreadsheet.getSheetByName("Entfernungen");
  var count = distanceSheet.getRange(1, 1, 1, 1).getValue();
  var coords = distanceSheet.getRange(2, 2, count + 1, 2).getDisplayValues();
  Logger.log("Coords size", coords.length, coords[0].length);
  var distanceRange = distanceSheet.getRange(2, 6, count + 1, count + 1);
  
  for (var i = 0; i < count + 1; ++i) {
    for (var j = 0; j < count + 1; ++j) {
      if (coords[i][0] == coords[j][0] && coords[i][1] == coords[j][1] ) {
        Logger.log("Found duplicate coords", coords[i][0], coords[i][1]);
        distanceRange.getCell(i + 1, j + 1).setValue(0);
        distanceRange.getCell(j + 1, i + 1).setValue(0);
      }
    }
  }
};

// =================================================

function computeDistance(directionFinder, startLat, startLon, endLat, endLon, cell) {
  Logger.log("computeDistance", startLat, startLon);
  directionFinder.setOrigin(startLat, startLon);
  directionFinder.setDestination(endLat, endLon);
  directionFinder.setMode(Maps.DirectionFinder.Mode.DRIVING);
  
  var directions = directionFinder.getDirections(); 
  var route = directions.routes[0];

  cell.setValue( route.legs[0].duration.value );
}

function localToFloat(s) {
  var str = s.toString();
  str = str.replace(/,/g, ".");
  return parseFloat(str);
}

function computeDistances() {
  var activeSpreadsheet = SpreadsheetApp.getActiveSpreadsheet();
  var distanceSheet = activeSpreadsheet.getSheetByName("Entfernungen");
  var count = distanceSheet.getRange(1, 1, 1, 1).getValue();
  var coords = distanceSheet.getRange(2, 2, count + 1, 2).getDisplayValues();
  Logger.log("Coords size", coords.length, coords[0].length);
  var distanceRange = distanceSheet.getRange(2, 6, count + 1, count + 1);
  // var distanceValues = distanceRange.getDisplayValues();
  var directions = Maps.newDirectionFinder();

  //count = 5;
  for (var r = 0; r < count + 1; ++r) {
    for (var c = 0; c < count + 1; ++c) {
      if (distanceRange.getCell(r + 1, c + 1).isBlank() ) {
        Logger.log("compute distance", coords[r][0], coords[r][1]);
        computeDistance(directions,
                        localToFloat(coords[r][0]), localToFloat(coords[r][1]),
                        localToFloat(coords[c][0]), localToFloat(coords[c][1]),
                        distanceRange.getCell(r + 1, c + 1));
      }
    }
  }
}

// =================================================

function generateMenu() {
  var entries = [
    {
      name: "Geocode Selected Cells (Address to Latitude, Longitude)",
      functionName: "addressToCoordinates"
    },
    {
      name: "!!! WARNUNG !!! Entferungstabelle vorbereiten",
      functionName: "prepareDistanceSheet"
    },
    {
      name: "Entferung -> gleiche Orte suchen",
      functionName: "lookForSamePlaces"
    },
    {
      name: "Entferung -> Distanzen berechnen",
      functionName: "computeDistances"
    },
    {
      name: "Send Map via EMail",
      functionName: "sendMap"
    } ];
  return entries;
};

function onOpen() {
  SpreadsheetApp.getActiveSpreadsheet().addMenu('Geocode', generateMenu());
};