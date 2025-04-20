#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long index = hash(key, map->capacity);
    for (long i=0; i < map->capacity; i++) {
        long pos = (index + i) % map->capacity;
        Pair * pair = map->buckets[pos];

        if (pair == NULL || pair->key == NULL) {
            map->buckets[pos] = createPair(key, value);
            map->size++;
            map->current = pos;
            return;
        }
        if (pair->key != NULL && strcmp(pair->key, key) == 0) {
            return;
        }
    }

}

void enlarge(HashMap * map) {
    Pair ** old_buckets = map->buckets;
    long old_capacity = map->capacity;
    map->capacity *= 2;
    map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *));

    if (map->buckets == NULL) return;
    map->size = 0;
    for (long i = 0; i < old_capacity; i++) {
        if (old_buckets[i] != NULL && old_buckets[i]->key != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }
    free(old_buckets);
}


HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *) malloc(sizeof(HashMap));
    map->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
    map->size = 0;
    map->capacity = capacity;
    map->current = -1;

    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    long index = hash(key, map->capacity);

    for (long i = 0; i < map->capacity; i++) {
        long pos = (index + i) % map->capacity;
        Pair * pair = map->buckets[pos];

        if (pair == NULL) return;

        if (pair->key != NULL && strcmp(pair->key, key) == 0) {
            pair->key = NULL; 
            map->size--; 
            map->current = pos;
            return;
        }
    }

}

Pair * searchMap(HashMap * map,  char * key) {   
    long index = hash(key, map->capacity);

    for (long i = 0; i < map->capacity; i++) {
        long pos = (index + i) % map->capacity;
        Pair * pair = map->buckets[pos];
        if (pair == NULL) return NULL;
        if (pair->key != NULL && strcmp(pair->key, key) == 0) {
            map->current = pos;
            return pair;
        }
    }
    return NULL;}


Pair * firstMap(HashMap * map) {
    for (long i = 0; i < map->capacity; i++) {
        Pair * pair = map->buckets[i];
        if (pair != NULL && pair->key != NULL) {
            map->current = i;
            return pair;
        }
    }

    return NULL;
}

Pair * nextMap(HashMap * map) {

    for (long i = map->current + 1; i < map->capacity; i++) {
        Pair * pair = map->buckets[i];
        if (pair != NULL && pair->key != NULL) {
            map->current = i;
            return pair;
        }
    }

    return NULL;
}
