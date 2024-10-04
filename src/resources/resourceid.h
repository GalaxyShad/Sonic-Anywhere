#ifndef SONIC_ANYWHERE_RESOURCEID_H
#define SONIC_ANYWHERE_RESOURCEID_H

typedef struct ResourceID {
    int id;
    const char* name;
} ResourceID;

#define DEF_RESOURCE_ID(X) const ResourceID X = { __LINE__, #X };
DEF_RESOURCE_ID(MAP_SONIC)
DEF_RESOURCE_ID(MAP_CRAB)

#endif // SONIC_ANYWHERE_RESOURCEID_H
