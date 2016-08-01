#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "imdb.h"

using namespace std;

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const
{
  return !( (actorInfo.fd == -1) ||
	    (movieInfo.fd == -1) );
}

/**
 * Returns void pointer to i-th Actor record (i is a given subscript)
 */
const void *imdb::getIthActorRecord(const unsigned int i) const {
    int offset = ((int*)actorFile)[i + 1];
    return (void*) &((char*)actorFile)[offset];
}

/**
 * Returns void pointer to Actor record in question
 */
const void *imdb::getActorRecord(const string& name) const {
    return actorFile;
}

/**
 * Populates vector of films where a given player acted
 * Returns true if the player has been found, false otherwise
 *
 */
bool imdb::getCredits(const string& player, vector<film>& films) const {
    int num_actors = *(int*) actorFile;
    cout << "num actors: " << num_actors << endl;
    cout << "first actor: " << string((char*)getActorRecord(0)) << endl;
    return false;
}

/**
 * Populates vector of strings (players) - full cast for a given movie
 * Returns true if the movie has been found, false otherwise
 *
 */
bool imdb::getCast(const film& movie, vector<string>& players) const {
    return false;
}

imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM..
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
  return info.fileMap;
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}
