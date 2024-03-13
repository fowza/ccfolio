//
// Created by fred on 3/11/24.
//

#ifndef MEDIA_H
#define MEDIA_H

#include <odb/core.hxx>
#include <string>
#include <vector>

#pragma db object
class Media
{
public:
    Media() = default;

    // Accessors and mutators for simplicity
    int getId() const
    {
        return id;
    }
    std::string getTitle() const
    {
        return title;
    }
    std::string getOverview() const
    {
        return overview;
    }
    std::string getLongDescription() const
    {
        return longDescription;
    }
    std::string getPosterPath() const
    {
        return posterPath;
    }
    std::string getReleaseDate() const
    {
        return releaseDate;
    }
    float getPopularity() const
    {
        return popularity;
    }
    std::vector<std::string> getGenres() const
    {
        return genres;
    }
    std::string getImage() const
    {
        return image;
    }
    std::string getHomepage() const
    {
        return homepage;
    }
    std::vector<std::string> getLanguages() const
    {
        return languages;
    }
    bool getInProduction() const
    {
        return inProduction;
    }
    std::string getStatus() const
    {
        return status;
    }

private:
    friend class odb::access;

#pragma db id auto
    int id{};

    std::string title;
    std::string overview;
    std::string longDescription;
    std::string posterPath;
    std::string releaseDate;
    float popularity;

#pragma db value(std::vector<std::string>) type("TEXT")
    std::vector<std::string> genres;

    std::string image; // Path to the image
    std::string homepage;

#pragma db value(std::vector<std::string>) type("TEXT")
    std::vector<std::string> languages;

    bool inProduction;
    std::string status;
};

#endif // MEDIA_H
