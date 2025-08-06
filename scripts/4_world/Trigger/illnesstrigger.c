class IllnessRemovalTrigger : Trigger
{
    protected const float DELETE_DELAY = 10.0;
    float m_TimeSinceCreation = 0;
    bool m_TriggerDeactivated = false;

    MJO_MedKit_open_illness m_MJO_MedKit_Illness;

    override void EOnInit(IEntity other, int extra)
    {
        super.EOnInit(other, extra);
        SetExtents("-2 -2 -2", "2 2 2");
    }

    override void EOnFrame(IEntity other, float timeSlice)
    {
        super.EOnFrame(other, timeSlice);

        m_TimeSinceCreation += timeSlice;

        if (m_TimeSinceCreation >= DELETE_DELAY && !m_TriggerDeactivated)
        {
            m_TriggerDeactivated = true;
            NotifyPlayersTriggerDeactivated();
            GetGame().ObjectDelete(this);
        }
    }

    void InitTrigger(MJO_MedKit_open_illness parentMedKit)
    {
        m_MJO_MedKit_Illness = parentMedKit;
    }

    override protected bool CanAddObjectAsInsider(Object object)
    {
        PlayerBase player;
        if (Class.CastTo(player, object) && IsPlayerControlled(player))
        {
            return player.IsAlive();
        }
        return false;
    }

    override protected bool ShouldRemoveInsider(TriggerInsider insider)
    {
        Object object = insider.GetObject();
        PlayerBase player;
        if (Class.CastTo(player, object) && IsPlayerControlled(player))
        {
            return !player.IsAlive();
        }
        return true;
    }

    override void OnEnter(Object obj)
    {
        super.OnEnter(obj);
        PlayerBase player;
        if (Class.CastTo(player, obj) && IsPlayerControlled(player) && !m_TriggerDeactivated)
        {
            NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Illness Removed", "All your illnesses have been cured.", "");
        }
    }
	override protected void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		PlayerBase player = PlayerBase.Cast(insider.GetObject());
		if (!player || m_TriggerDeactivated)
			return;

		if (player.HasDisease())
		{
			RemoveAllIllnesses(player);
		}
	}
    override void OnLeave(Object obj)
    {
        super.OnLeave(obj);
        PlayerBase player;
        if (Class.CastTo(player, obj) && IsPlayerControlled(player) && !m_TriggerDeactivated)
        {
            NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Zone Left", "You left the illness removal zone.", "");
        }
    }

    void NotifyPlayersTriggerDeactivated()
    {
        array<ref TriggerInsider> insiders = GetInsiders();
        for (int i = 0; i < insiders.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(insiders[i].GetObject());
            if (player && IsPlayerControlled(player))
            {
                NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Zone Deactivated", "The illness removal zone has been deactivated.", "");
            }
        }
    }

	void RemoveAllIllnesses(PlayerBase player)
	{
		if (!player || !GetGame().IsServer())
			return;

		player.GetModifiersManager().DeactivateAllModifiers();

		while (player.HasDisease())
		{
			player.DecreaseDiseaseCount();
		}

		if (player.m_AgentPool)
		{
			player.m_AgentPool.RemoveAllAgents();
		}
	}
    protected bool IsPlayerControlled(PlayerBase player)
    {
        return player && player.GetIdentity();
    }
}