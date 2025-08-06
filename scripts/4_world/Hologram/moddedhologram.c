modded class Hologram
{
	override void EvaluateCollision(ItemBase action_item = null)
	{	
		if ( isKit() )
		{
			SetIsColliding(false);
			return;
		}

		super.EvaluateCollision( action_item );
	}

	bool isKit()
	{
		if ( m_Parent.IsInherited( MJO_MedKit_close_Kit ))
		{
			return true;
		}

		return false;
	}
	override protected vector GetProjectionEntityPosition(PlayerBase player)
	{
		if ( !isKit() )
		{
			return super.GetProjectionEntityPosition(player);
		}
		
		float minProjectionDistance;
		float maxProjectionDistance; 
		m_ContactDir = vector.Zero;
		vector minMax[2];
		float projectionRadius = GetProjectionRadius();
		float cameraToPlayerDistance = vector.Distance(GetGame().GetCurrentCameraPosition(), player.GetPosition());

		if (projectionRadius < SMALL_PROJECTION_RADIUS)
		{
			minProjectionDistance = SMALL_PROJECTION_RADIUS;
			maxProjectionDistance = SMALL_PROJECTION_RADIUS * 2;
		}
		else
		{
			minProjectionDistance = projectionRadius;
			maxProjectionDistance = projectionRadius * 2;
			maxProjectionDistance = Math.Clamp(maxProjectionDistance, SMALL_PROJECTION_RADIUS, LARGE_PROJECTION_DISTANCE_LIMIT);
		}
		
		vector from = GetGame().GetCurrentCameraPosition();
		if (DayZPlayerCamera3rdPerson.Cast(player.GetCurrentCamera()))
		{
			vector head_pos;
			MiscGameplayFunctions.GetHeadBonePos(player,head_pos);
			float dist = vector.Distance(head_pos,from);
			from = from + GetGame().GetCurrentCameraDirection() * dist;
		}
		
		vector to = from + (GetGame().GetCurrentCameraDirection() * (maxProjectionDistance + cameraToPlayerDistance));
		vector contactPosition;
		set<Object> hitObjects = new set<Object>();

		DayZPhysics.RaycastRV(from, to, contactPosition, m_ContactDir, m_ContactComponent, hitObjects, player, m_Projection, false, false, ObjIntersectFire);
		
		static const float raycastOriginOffsetOnFail = 0.25;
		static const float minDistFromStart = 0.01;
		// Camera isn't correctly positioned in some cases, leading to raycasts hitting the object directly behind the camera
		if ((hitObjects.Count() > 0) && (vector.DistanceSq(from, contactPosition) < minDistFromStart))
		{
			from = contactPosition + GetGame().GetCurrentCameraDirection() * raycastOriginOffsetOnFail;
			DayZPhysics.RaycastRV(from, to, contactPosition, m_ContactDir, m_ContactComponent, hitObjects, player, m_Projection, false, false, ObjIntersectFire);
		}
		if ((hitObjects.Count() > 0) && hitObjects[0].IsInherited(Watchtower))
			contactPosition = CorrectForWatchtower( m_ContactComponent, contactPosition, player, hitObjects[0] );
		bool isFloating = SetHologramPosition(player.GetPosition(), minProjectionDistance, maxProjectionDistance, contactPosition);
		SetIsFloating(isFloating);		
		m_FromAdjusted = from;
		return contactPosition;
	}
}