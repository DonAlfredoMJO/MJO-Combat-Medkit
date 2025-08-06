class HealingTrigger : Trigger
{
    protected const float HEAL_AMOUNT = 3.0;
    protected const float TICK_INTERVAL = 1.0;
    protected const float DELETE_DELAY = 10.0;

    MJO_MedKit_open m_MJO_MedKit;
    float m_TimeSinceCreation = 0;
    bool m_TriggerDeactivated = false;

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
            m_TriggerDeactivated = true; // Mark the trigger as deactivated
            NotifyPlayersTriggerDeactivated(); // Notify players in the zone
            GetGame().ObjectDelete(this); // Delete the trigger
        }
    }

    void InitTrigger(MJO_MedKit_open parentMedKit)
    {
        m_MJO_MedKit = parentMedKit;
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

    override protected void OnStayServerEvent(TriggerInsider insider, float deltaTime)
    {
        super.OnStayServerEvent(insider, deltaTime); //changed from OnStayBeginEvent to OnStayServerEvent - works like that! --> OnStayBeginEvent is client and server!

        PlayerBase player = PlayerBase.Cast(insider.GetObject());
        if (player && !m_TriggerDeactivated) // Only apply healing if the trigger is active
        {
            player.AddHealth("", "", HEAL_AMOUNT * deltaTime / TICK_INTERVAL);
        }
    }

    override void OnEnter(Object obj)
    {
        super.OnEnter(obj);
        PlayerBase player;
        if (Class.CastTo(player, obj) && IsPlayerControlled(player) && !m_TriggerDeactivated)
        {
            NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Healing Active", "You will now be healed by 3 HP per second.", "");
        }
    }

    override void OnLeave(Object obj)
    {
        super.OnLeave(obj);
        PlayerBase player;
        if (Class.CastTo(player, obj) && IsPlayerControlled(player) && !m_TriggerDeactivated)
        {
            NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Healing Stopped", "You left the healing zone.", "");
        }
    }

    void NotifyPlayersTriggerDeactivated()
    {
        // Cast the return value of GetInsiders() to match the correct type
        array<ref TriggerInsider> insiders = GetInsiders();
        for (int i = 0; i < insiders.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(insiders[i].GetObject());
            if (player && IsPlayerControlled(player))
            {
                NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Healing Ended", "The healing zone is now deactivated.", "");
            }
        }
    }

    protected bool IsPlayerControlled(PlayerBase player)
    {
        return player && player.GetIdentity();
    }
}