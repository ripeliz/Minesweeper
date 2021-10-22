#include "Textures.h"

unordered_map<string, sf::Texture> Textures::textures;

void Textures::LoadTexture(string fileName) {
	string path = "images/" + fileName + ".png";

	textures[fileName].loadFromFile(path);
}

sf::Texture& Textures::GetTexture(string textureName) {
	if (textures.find(textureName) == textures.end()) {
		LoadTexture(textureName);
	}
	return textures[textureName];
}

void Textures::Clear() {
	textures.clear();
}


