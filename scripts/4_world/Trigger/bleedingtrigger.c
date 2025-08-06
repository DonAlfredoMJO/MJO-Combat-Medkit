class BleedingTrigger : Trigger
{
    protected const float TICK_INTERVAL = 1.0;
    protected const float DELETE_DELAY = 10.0;

    MJO_MedKit_open_bleeds m_MJO_MedKit_Bleeds;
    float m_TimeSinceCreation = 0;
    bool m_TriggerDeactivated = false;

    override void EOnInit(IEntity other, int extra)
    {
        SetExtents("-2 -2 -2", "2 2 2");
    }

    override void EOnFrame(IEntity other, float timeSlice)
    {

        m_TimeSinceCreation += timeSlice;

        if (m_TimeSinceCreation >= DELETE_DELAY && !m_TriggerDeactivated)
        {
            m_TriggerDeactivated = true;
            NotifyPlayersTriggerDeactivated();
            GetGame().ObjectDelete(this);
        }
    }

    void InitTrigger(MJO_MedKit_open_bleeds parentMedKit)
    {
        m_MJO_MedKit_Bleeds = parentMedKit;
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

    override protected void OnStayBeginEvent(TriggerInsider insider, float deltaTime)
    {
        super.OnStayBeginEvent(insider, deltaTime);

        PlayerBase player = PlayerBase.Cast(insider.GetObject());
        if (player && !m_TriggerDeactivated)
        {
            if (player.GetBleedingManagerServer())
            {
                for (int i = 0; i < 6; i++)
                {
                    player.GetBleedingManagerServer().RemoveMostSignificantBleedingSource();
                }
            }
        }
    }

    override void OnEnter(Object obj)
    {
        super.OnEnter(obj);
        PlayerBase player;
        if (Class.CastTo(player, obj) && IsPlayerControlled(player) && !m_TriggerDeactivated)
        {
            NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Bleeding Stopped", "You are now being treated for cuts.", "");
        }
    }

    override void OnLeave(Object obj)
    {
        super.OnLeave(obj);
        PlayerBase player;
        if (Class.CastTo(player, obj) && IsPlayerControlled(player) && !m_TriggerDeactivated)
        {
            NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Treatment Stopped", "You left the treatment zone.", "");
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
                NotificationSystem.SendNotificationToPlayerExtended(player, 2, "Treatment Ended", "The treatment zone is now deactivated.", "");
            }
        }
    }

    protected bool IsPlayerControlled(PlayerBase player)
    {
        return player && player.GetIdentity();
    }
}