using System;
using System.Runtime.InteropServices;

class MidiFighterTwister
{
#if (UNITY_STANDALONE_WIN && UNITY_64) || (UNITY_EDITOR_WIN && UNITY_64) || WIN64
    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftStart")]
    public static extern byte mftStart();

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftStop")]
    public static extern void mftStop();

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftIsUpdatedKnobValue")]
    public static extern byte mftIsUpdatedKnobValue(byte idx);

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftGetKnobValue")]
    public static extern byte mftGetKnobValue(byte idx);

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftSetKnobValue")]
    public static extern void mftSetKnobValue(byte idx, byte value);

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftIsUpdatedButtonValue")]
    public static extern byte mftIsUpdatedButtonValue(byte idx);

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftGetButtonValue")]
    public static extern byte mftGetButtonValue(byte idx);

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftSetKnobColor")]
    public static extern void mftSetKnobColor(byte idx, byte value);

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftIsUpdatedBank")]
    public static extern byte mftIsUpdatedBank();

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftGetBank")]
    public static extern byte mftGetBank();

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftSetBank")]
    public static extern void mftSetBank(byte bank_idx);

    [DllImport("MidiFighterTwister64.dll", EntryPoint = "mftSendCCMessage")]
    public static extern void mftSendCCMessage(byte channel, byte control_number, byte control_data);
#elif  (UNITY_STANDALONE_WIN && UNITY_32) || (UNITY_EDITOR_WIN && UNITY_32) || !WIN64
    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftStart")]
    public static extern byte mftStart();

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftStop")]
    public static extern void mftStop();

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftIsUpdatedKnobValue")]
    public static extern byte mftIsUpdatedKnobValue(byte idx);

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftGetKnobValue")]
    public static extern byte mftGetKnobValue(byte idx);

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftSetKnobValue")]
    public static extern void mftSetKnobValue(byte idx, byte value);

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftIsUpdatedButtonValue")]
    public static extern byte mftIsUpdatedButtonValue(byte idx);

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftGetButtonValue")]
    public static extern byte mftGetButtonValue(byte idx);

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftSetKnobColor")]
    public static extern void mftSetKnobColor(byte idx, byte value);

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftIsUpdatedBank")]
    public static extern byte mftIsUpdatedBank();

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftGetBank")]
    public static extern byte mftGetBank();

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftSetBank")]
    public static extern void mftSetBank(byte bank_idx);

    [DllImport("MidiFighterTwister32.dll", EntryPoint = "mftSendCCMessage")]
    public static extern void mftSendCCMessage(byte channel, byte control_number, byte control_data);
#else

#endif

    public static bool Start()
    {
        return mftStart() > 0 ? true : false;
    }

    public static void Stop()
    {
        mftStop();
    }

    public static bool IsUpdatedKnobValue(byte idx)
    {
        return mftIsUpdatedKnobValue(idx) > 0 ? true : false;
    }

    public static byte GetKnobValue(byte idx)
    {
        return mftGetKnobValue(idx);
    }

    public static void SetKnobValue(byte idx, byte value)
    {
        mftSetKnobValue(idx, value);
    }

    public static bool IsUpdatedButtonValue(byte idx)
    {
        return mftIsUpdatedButtonValue(idx) > 0 ? true : false;
    }

    public static byte GetButtonValue(byte idx)
    {
        return mftGetButtonValue(idx);
    }

    public static void SetKnobColor(byte idx, byte value)
    {
        mftSetKnobColor(idx, value);
    }

    public static bool IsUpdatedBank()
    {
        return mftIsUpdatedBank() > 0 ? true : false;
    }

    public static byte GetBank()
    {
        return mftGetBank();
    }

    public static void SetBank(byte bank_idx)
    {
        mftSetBank(bank_idx);
    }

    public static void SendCCMessage(byte channel, byte control_number, byte control_data)
    {
        mftSendCCMessage(channel, control_number, control_data);
    }
}

