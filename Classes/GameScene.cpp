#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// Create a tmx map
	this->map = TMXTiledMap::create("forest.tmx");
	addChild(map, 0, -1);
	
	// all tiles are aliased by default, let's set them anti-aliased
	for (const auto& child : map->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
	
}

void GameScene::prepareLayers()
{
	for (auto& object : this->map->getChildren())
	{
		// is this map child a tile layer?
		auto layer = dynamic_cast<TMXLayer*>(object);
		if (layer != nullptr)
			this->createFixtures(layer);
	}
}

void GameScene::createFixtures(TMXLayer* layer)
{
	// create all the rectangular fixtures for each tile
	Size layerSize = layer->getLayerSize();
	for (int y = 0; y < layerSize.height; y++)
	{
		for (int x = 0; x < layerSize.width; x++)
		{
			// create a fixture if this tile has a sprite
			auto tileSprite = layer->getTileAt(Point(x, y));
			if (tileSprite)
				this->createRectangularFixture(layer, x, y, 1.1f, 1.1f);
		}
	}
}

void GameScene::createRectangularFixture(TMXLayer* layer, int x, int y, float width, float height)
{
	// get position & size
	auto p = layer->getPositionAt(Point(x,y));
	auto tileSize = this->map->getTileSize();
	const float pixelsPerMeter = 32.0f;
	
	// note: creating the 'world' member variable
	// is discussed in the next chapter
	
	// create the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(
						 (p.x + (tileSize.width / 2.0f)) / pixelsPerMeter,
						 (p.y + (tileSize.height / 2.0f)) / pixelsPerMeter
						 );
	b2Body* body = world->CreateBody(&bodyDef);
	
	// define the shape
	b2PolygonShape shape;
	shape.SetAsBox(
				   (tileSize.width / pixelsPerMeter) * 0.5f * width,
				   (tileSize.width / pixelsPerMeter) * 0.5f * height
				   );
	
	// create the fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = kFilterCategoryLevel;
	fixtureDef.filter.maskBits = 0xffff;
	body->CreateFixture(&fixtureDef);
}
