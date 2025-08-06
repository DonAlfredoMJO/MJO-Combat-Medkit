class MJO_MedKit_close_Kit extends ItemBase
{

    HealingTrigger m_HealingTrigger;
    bool m_TriggerCreated = false;
	
	override bool IsDeployable()
	{
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}	
	
	override void OnPlacementStarted(Man player)
    {
        super.OnPlacementStarted(player);

        // Retrieve hiddenSelections config if it exists
        if (ConfigIsExisting("hiddenSelections"))
        {
            PlayerBase playerBase = PlayerBase.Cast(player);
            TStringArray hiddenSelections = new TStringArray();
            ConfigGetTextArray("hiddenSelections", hiddenSelections);

            // Determine if we're in multiplayer and server-side or not
            bool isMultiplayerServer = GetGame().IsMultiplayer() && GetGame().IsServer();

            for (int i = 0; i < hiddenSelections.Count(); i++)
            {
                string selection = hiddenSelections.Get(i);

                // Select the hologram to refresh based on the context (server or client)
                if (isMultiplayerServer)
                {
                    playerBase.GetHologramServer().SetSelectionToRefresh(selection);
                }
                else
                {
                    playerBase.GetHologramLocal().SetSelectionToRefresh(selection);
                }
            }
        }
    }
	
	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);

		if (GetGame().IsServer() && ConfigIsExisting("projectionTypename"))
		{
			string projectionTypename = ConfigGetString("projectionTypename");

			if (projectionTypename != "")
			{
				ItemBase placedItem = ItemBase.Cast(GetGame().CreateObjectEx(projectionTypename, position, ECE_PLACE_ON_SURFACE));
				MJO_MedKit_open medkit = MJO_MedKit_open.Cast(placedItem);

				if (medkit)
				{
					// Set position and orientation of the newly created object
					medkit.SetPosition(position);
					medkit.SetOrientation(orientation);

					HideAllSelections();

					if (!m_TriggerCreated)
					{
						m_TriggerCreated = true;
						if (Class.CastTo(m_HealingTrigger, GetGame().CreateObjectEx("HealingTrigger", position, ECE_CREATEPHYSICS)))
						{
							m_HealingTrigger.SetPosition(position);
							m_HealingTrigger.InitTrigger(medkit);
						}
						else
						{
							m_TriggerCreated = false;
						}
					}
				}

				if (GetGame().IsServer() || GetGame().IsClient())
				{
					GetGame().ObjectDelete(this);
				}
			}
		}
	}
}	
class MJO_MedKit_open extends ItemBase
{
	
	override void EEInit()
	{
		super.EEInit();
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DeleteSelf, 15000, false);
		}
	}

    void DeleteSelf()
    {
        GetGame().ObjectDelete(this);
    }

    override bool IsTakeable()
    {
        return false;
    }

    override bool CanSwapEntities(EntityAI otherItem, InventoryLocation otherDestination, InventoryLocation destination)
    {
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }

    override bool CanPutAsAttachment(EntityAI parent)
    {
        return false;
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }
}

class MJO_MedKit_close_bleed_Kit extends ItemBase
{

    BleedingTrigger m_BleedingTrigger;
    bool m_BleedingTriggerCreated = false;
	
	override bool IsDeployable()
	{
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}	
	
	override void OnPlacementStarted(Man player)
    {
        super.OnPlacementStarted(player);

        // Retrieve hiddenSelections config if it exists
        if (ConfigIsExisting("hiddenSelections"))
        {
            PlayerBase playerBase = PlayerBase.Cast(player);
            TStringArray hiddenSelections = new TStringArray();
            ConfigGetTextArray("hiddenSelections", hiddenSelections);

            // Determine if we're in multiplayer and server-side or not
            bool isMultiplayerServer = GetGame().IsMultiplayer() && GetGame().IsServer();

            for (int i = 0; i < hiddenSelections.Count(); i++)
            {
                string selection = hiddenSelections.Get(i);

                // Select the hologram to refresh based on the context (server or client)
                if (isMultiplayerServer)
                {
                    playerBase.GetHologramServer().SetSelectionToRefresh(selection);
                }
                else
                {
                    playerBase.GetHologramLocal().SetSelectionToRefresh(selection);
                }
            }
        }
    }	

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
        super.OnPlacementComplete(player, position, orientation);

        if (GetGame().IsServer() && ConfigIsExisting("projectionTypename"))
        {
            string projectionTypename = ConfigGetString("projectionTypename");
        
            if (projectionTypename != "")
            {
                ItemBase placedItem = ItemBase.Cast(GetGame().CreateObjectEx(projectionTypename, position, ECE_PLACE_ON_SURFACE));
				MJO_MedKit_open_bleeds medkit1 = MJO_MedKit_open_bleeds.Cast(placedItem);
				
                if (medkit1)
                {
                    // Set position and orientation of the newly created object
                    medkit1.SetPosition(position);
                    medkit1.SetOrientation(orientation);

                    HideAllSelections();
					
					if (!m_BleedingTriggerCreated)
					{
						m_BleedingTriggerCreated = true;
						if (Class.CastTo(m_BleedingTrigger, GetGame().CreateObjectEx("BleedingTrigger", position, ECE_CREATEPHYSICS)))
						{
							m_BleedingTrigger.SetPosition(position);
							m_BleedingTrigger.InitTrigger(medkit1);
						}
						else
						{
							m_BleedingTriggerCreated = false;
						}
					}					
                }
				if (GetGame().IsServer() || GetGame().IsClient())
				{
					GetGame().ObjectDelete(this);
				}				
            }
		}
	}
}
class MJO_MedKit_open_bleeds extends ItemBase
{
	
	override void EEInit()
	{
		super.EEInit();
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DeleteSelf, 15000, false);
		}
	}

    void DeleteSelf()
    {
        GetGame().ObjectDelete(this);
    }

    override bool IsTakeable()
    {
        return false;
    }

    override bool CanSwapEntities(EntityAI otherItem, InventoryLocation otherDestination, InventoryLocation destination)
    {
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }

    override bool CanPutAsAttachment(EntityAI parent)
    {
        return false;
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }
}

class MJO_MedKit_close_illness_Kit extends ItemBase
{

    IllnessRemovalTrigger m_IllnessRemovalTrigger;
    bool m_IllnessRemovalTriggerCreated = false;
	
	override bool IsDeployable()
	{
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}	
	
	override void OnPlacementStarted(Man player)
    {
        super.OnPlacementStarted(player);

        // Retrieve hiddenSelections config if it exists
        if (ConfigIsExisting("hiddenSelections"))
        {
            PlayerBase playerBase = PlayerBase.Cast(player);
            TStringArray hiddenSelections = new TStringArray();
            ConfigGetTextArray("hiddenSelections", hiddenSelections);

            // Determine if we're in multiplayer and server-side or not
            bool isMultiplayerServer = GetGame().IsMultiplayer() && GetGame().IsServer();

            for (int i = 0; i < hiddenSelections.Count(); i++)
            {
                string selection = hiddenSelections.Get(i);

                // Select the hologram to refresh based on the context (server or client)
                if (isMultiplayerServer)
                {
                    playerBase.GetHologramServer().SetSelectionToRefresh(selection);
                }
                else
                {
                    playerBase.GetHologramLocal().SetSelectionToRefresh(selection);
                }
            }
        }
    }	

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
        super.OnPlacementComplete(player, position, orientation);

        if (GetGame().IsServer() && ConfigIsExisting("projectionTypename"))
        {
            string projectionTypename = ConfigGetString("projectionTypename");
        
            if (projectionTypename != "")
            {
                ItemBase placedItem = ItemBase.Cast(GetGame().CreateObjectEx(projectionTypename, position, ECE_PLACE_ON_SURFACE));
				MJO_MedKit_open_illness medkit2 = MJO_MedKit_open_illness.Cast(placedItem);
				
                if (medkit2)
                {
                    // Set position and orientation of the newly created object
                    medkit2.SetPosition(position);
                    medkit2.SetOrientation(orientation);

                    HideAllSelections();
					
					if (!m_IllnessRemovalTriggerCreated)
					{
						m_IllnessRemovalTriggerCreated = true;
						if (Class.CastTo(m_IllnessRemovalTrigger, GetGame().CreateObjectEx("IllnessRemovalTrigger", position, ECE_CREATEPHYSICS)))
						{
							m_IllnessRemovalTrigger.SetPosition(position);
							m_IllnessRemovalTrigger.InitTrigger(medkit2);
						}
						else
						{
							m_IllnessRemovalTriggerCreated = false;
						}
					}					
                }
				if (GetGame().IsServer() || GetGame().IsClient())
				{
					GetGame().ObjectDelete(this);
				}				
            }
		}
	}
}
class MJO_MedKit_open_illness extends ItemBase
{
	
	override void EEInit()
	{
		super.EEInit();
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DeleteSelf, 15000, false);
		}
	}

    void DeleteSelf()
    {
        GetGame().ObjectDelete(this);
    }

    override bool IsTakeable()
    {
        return false;
    }

    override bool CanSwapEntities(EntityAI otherItem, InventoryLocation otherDestination, InventoryLocation destination)
    {
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }

    override bool CanPutAsAttachment(EntityAI parent)
    {
        return false;
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }
}