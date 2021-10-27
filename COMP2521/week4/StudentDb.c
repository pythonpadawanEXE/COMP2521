// Implementation of the Student DB ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "StudentDb.h"
#include "Tree.h"

struct studentDb {
    Tree byZid;
    Tree byName;
};

////////////////////////////////////////////////////////////////////////
// Comparison functions

/**
 * Compares two records by zid only and returns:
 * - A negative number if the first record is less than the second
 * - Zero if the records are equal
 * - A positive number if the first record is greater than the second
 */
int compareByZid(Record r1, Record r2) {
    return RecordGetZid(r1) - RecordGetZid(r2);
}

/**
 * Compares two records by name (family name first) and then by
 * zid if the names are equal, and returns:
 * - A negative number if the first record is less than the second
 * - Zero if the records are equal
 * - A positive number if the first record is greater than the second
 */
int compareByName(Record r1, Record r2) {
    
    int cmp = strcmp(RecordGetFamilyName(r1), RecordGetFamilyName(r2));
	
	// if names are not the same, return the result of strcmp
	if (cmp != 0) {
		return cmp;
	}
	//if names are the same cmp first/given name
	cmp = strcmp(RecordGetGivenName(r1), RecordGetGivenName(r2));
	// if names are not the same, return the result of strcmp
	if (cmp != 0) {
		return cmp;
	}
	// names are the same, so compare zids
    else {
		return compareByZid(r1, r2);
	}
}

////////////////////////////////////////////////////////////////////////

StudentDb DbNew(void) {
    StudentDb db = malloc(sizeof(*db));
    if (db == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    db->byZid = TreeNew(compareByZid);
    // TODO: Modify this line to use compareByName once you have
    //       implemented it
    db->byName = TreeNew(compareByName);
    return db;
}

void DbFree(StudentDb db) {
    TreeFree(db->byZid, false);
    TreeFree(db->byName, true);
    free(db);
}

////////////////////////////////////////////////////////////////////////

bool DbInsertRecord(StudentDb db, Record r) {
    if (TreeInsert(db->byZid, r)) {
        TreeInsert(db->byName, r);
        return true;
    } else {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////

bool DbDeleteByZid(StudentDb db, int zid) {
    Record dummy = RecordNew(zid, "", "");
    Record r = TreeSearch(db->byZid, dummy);

    if (r != NULL) {
        TreeDelete(db->byZid, r);
        TreeDelete(db->byName, r);
        RecordFree(r);
        RecordFree(dummy);
        return true;
    } else {
        RecordFree(dummy);
        return false;
    }
}

////////////////////////////////////////////////////////////////////////

Record DbFindByZid(StudentDb db, int zid) {
    Record dummy = RecordNew(zid, "", "");
    Record r = TreeSearch(db->byZid, dummy);
    RecordFree(dummy);
    return r;
}

////////////////////////////////////////////////////////////////////////

List DbFindByName(StudentDb db, char *familyName, char *givenName) {
    // TODO: Complete this function
    Record dummy1 = RecordNew(1, familyName, givenName);
    Record dummy2 = RecordNew(9999999, familyName, givenName);
    //search between all zids which contain family name and given name
    List l = TreeSearchBetween(db->byName, dummy1,dummy2);
    RecordFree(dummy1);
    RecordFree(dummy2);
    return l;
}

////////////////////////////////////////////////////////////////////////

void DbListByZid(StudentDb db) {
    TreeListInOrder(db->byZid);
}

void DbListByName(StudentDb db) {
    TreeListInOrder(db->byName);
}
