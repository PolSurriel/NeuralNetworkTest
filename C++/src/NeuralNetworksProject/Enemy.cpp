#include "Enemy.h"
#include "Consts.h"
#include "Time.h"
#include "PlayableCharacter.h"
#include <math.h>

Enemy::Enemy() {
	tag = Tag::ENEMY;
	movingDirection = ENEMY_INITIAL_DIRECTION;

	components.push_back(new SquareCollider(this, SQUARE_WIDTH_WORLD_SPACE));

	changeDirectionCooldownCount = 0;
	
}

void Enemy::Update(const InputManager& input) {


	if (freezedPos) {
		
		std::cout << "doing" << std::endl;

		unfreezeTimeCount += FPSTime::deltaTimeInMilliseconds / 1000;

		if (unfreezeTimeCount >= TO_UNFREEZE_POSITION) {
			freezedPos = false;
			rigidBody2D->freezePosition = false;
			std::cout << "DONE" << std::endl;
			
		}
	}

	if (position.x > MAP_WIDTH)
	{
		position.x = -SQUARE_WIDTH_WORLD_SPACE;
	}
	else if (position.x < -SQUARE_WIDTH_WORLD_SPACE)
	{
		position.x = MAP_WIDTH;
	}

	if (position.y > MAP_HEIGHT)
	{
		position.y = -SQUARE_HEIGHT_WORLD_SPACE;
	}
	else if (position.y < -SQUARE_HEIGHT_WORLD_SPACE)
	{
		position.y = MAP_HEIGHT;
	}


	desiredDirection = Vector2::Perpendicular(movingDirection) * ((rand() % 2) ? 1 : -1);

	//rigidBody2D->velocity = movingDirection * speed;
	rigidBody2D->velocity = VECTOR2_LEFT * speed;


	if (movingDirection == VECTOR2_ZERO)
		movingDirection = desiredDirection;


	GetComponent<SquareCollider>()->width = SQUARE_WIDTH_WORLD_SPACE * 0.99;

	rigidBody2D->velocity = movingDirection * speed;


std::list<Collider*> hits = Collider::RayCastStatic(position + Vector2(SQUARE_RADIO_WORLD_SPACE, SQUARE_RADIO_WORLD_SPACE), desiredDirection, SQUARE_WIDTH_WORLD_SPACE, GetId());

//Mehorar calculo del origen del hit 2
std::list<Collider*> hits2 = Collider::RayCastStatic(movingDirection * SQUARE_RADIO_WORLD_SPACE*0.99f + position + Vector2(SQUARE_RADIO_WORLD_SPACE, SQUARE_RADIO_WORLD_SPACE), desiredDirection, SQUARE_WIDTH_WORLD_SPACE, GetId());

if (changeDirectionCooldownCount > 0)
{
	changeDirectionCooldownCount -= FPSTime::deltaTimeInMilliseconds / 1000;
}

if (rand() % 100 <= changeDirectionProbability && desiredDirection != movingDirection && hits.empty() && hits2.empty())
{
	if (desiredDirection == movingDirection * -1) {
		movingDirection = desiredDirection;
	}
	else if (changeDirectionCooldownCount <= 0) {

		Vector2 railPosition(
			trunc(position.x / SQUARE_WIDTH_WORLD_SPACE),
			trunc(position.y / SQUARE_HEIGHT_WORLD_SPACE)
		);

		railPosition = (railPosition)* SQUARE_WIDTH_WORLD_SPACE;

		if (movingDirection == VECTOR2_RIGHT) {
			railPosition.x += SQUARE_WIDTH_WORLD_SPACE;
			railPosition.x -= DISTANCE_BETWEEN_WALL_AND_PACMAN_RD;

		}

		else if (movingDirection == VECTOR2_DOWN) {
			railPosition.y += SQUARE_WIDTH_WORLD_SPACE;
			railPosition.y -= DISTANCE_BETWEEN_WALL_AND_PACMAN_RD;
		}

		else if (movingDirection == VECTOR2_UP) {
			railPosition.y += DISTANCE_BETWEEN_WALL_AND_PACMAN_UL;
		}

		else if (movingDirection == VECTOR2_LEFT) {
			railPosition.x += DISTANCE_BETWEEN_WALL_AND_PACMAN_UL;
		}

		float distanceToMove = speed * FPSTime::deltaTimeInMilliseconds;
		float distanceToRail = (railPosition - position + desiredDirection * -SQUARE_RADIO_WORLD_SPACE).GetMagnitude();


		if (distanceToMove <= distanceToRail) {
			movingDirection = desiredDirection;
			position = railPosition;
			changeDirectionCooldownCount = CHANGE_DIRECTION_COOLDOWN;
		}
	}
}


UpdateSprite();
}

void Enemy::UpdateSprite() {
	/*millisecondsInLife += FPSTime::deltaTimeInMilliseconds;

	int animationIndex;
	AnimatedSprite::Type spriteDirection;*/

	switch (state)
	{
	case EnemyState::Running:
		Entity::UpdateSprite();
		break;
	case EnemyState::EvadingPlayer:
		millisecondsInLife += FPSTime::deltaTimeInMilliseconds;
		int animationIndex = (static_cast<int>(millisecondsInLife) / MILLISECONDS_TO_UPDATE_SPRITE) % NUM_SPRITE_GHOST_EXTRA_FRAMES;
		AnimatedSprite::Type spriteDirection = AnimatedSprite::Type::DISINTEGRATION;

		animatedSprite.SetTexture(spriteDirection, animationIndex);
		break;
	}
}

void Enemy::UpdateDirection() {
	movingDirection = desiredDirection;
	/*movingDirection.x = cos(angleDirection) * velocity;
	movingDirection.y = sin(angleDirection) * velocity;*/
}

void Enemy::OnCollision(Collider& _collider)
{
	switch (_collider.gameObject->tag)
	{
	case GameObject::Tag::WALL:
		position = Collision::SquareToSquareReaction(
			position, SQUARE_WIDTH_WORLD_SPACE / 2, rigidBody2D->velocity,
			_collider.gameObject->position, SQUARE_WIDTH_WORLD_SPACE / 2
		);
		break;

	case Tag::PLAYABLE_CHARACTER:

		if (((PlayableCharacter*)_collider.gameObject)->type == PlayableCharacter::Type::PERSECUTOR){

			
			
		}


		if(((PlayableCharacter*)_collider.gameObject)->state == PlayableCharacterState::PoweredUp)
		{
			Destroy(this);
		}
		break;
	default:
		break;
	}
}



void Enemy::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
{
	switch (evt_info.first)
	{
	case Event::MAINPLAYER_GETS_DAMAGE:
		rigidBody2D->freezePosition = true;
		break;
	case Event::POWER_UP_GOTTEN:
		state = EnemyState::EvadingPlayer;
		break;
	case Event::POWER_UP_TIME_OVER:
		state = EnemyState::Running;
		break;

	case Event::RESET_POSITIONS:
		position = initialPosition;
		rigidBody2D->freezePosition = false;

	break;
		
	}
}


EnemySpawnManager::EnemySpawnManager(const Vector2& _initialPosition) : initialPosition(_initialPosition)
{
	state = State::WAITING_TO_INSTANTIATE;
	timeToSpawnCount = TIME_TO_SPAWN;
}

void EnemySpawnManager::SetSprite(AnimatedSprite* &sprite) {
	animatedSprite = sprite;
}


void EnemySpawnManager::Update(const InputManager& input)
{
	
	if(state == State::WAITING_TO_INSTANTIATE)
	{
		timeToSpawnCount += FPSTime::deltaTimeInMilliseconds /1000;
		if(timeToSpawnCount >= TIME_TO_SPAWN)
		{
			state = State::NORMAL;
			instantiated = new Enemy;
			instantiated->initialPosition = initialPosition;
			instantiated->position = initialPosition;
			instantiated->SetSprite(animatedSprite);
			instantiated->state = spawnState;
			//instantiated->actualSprite = actualSprite;
			
			Instantiate(instantiated, DRAWABLE | DYNAMIC);
			
		}
	}
	
}

void EnemySpawnManager::HandleEvent(std::pair<Event, stream_buff::buffer> evt_info)
{
	switch (evt_info.first)
	{
		case Event::ENEMY_KILLED:
			state = State::WAITING_TO_INSTANTIATE;
			timeToSpawnCount = 0;
			break;
		case Event::POWER_UP_GOTTEN:
			spawnState = EnemyState::EvadingPlayer;
			break;
		case Event::POWER_UP_TIME_OVER:
			spawnState = EnemyState::Running;
			break;
		
	}
	
}

EnemySpawnManager::~EnemySpawnManager()
{
	if(state == State::NORMAL)
	{
		delete instantiated;
	}
}



