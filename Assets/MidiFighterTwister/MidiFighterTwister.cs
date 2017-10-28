//
//  MidiFighterTwister.cs - Midi Fighter Twister Controller class for Unity
//
//	github:
//		https://github.com/yoggy/MidiFighterTwister
//
//  license:
//      Copyright(c) 2017 yoggy <yoggy0@gmail.com>
//      Released under the MIT license
//      http://opensource.org/licenses/mit-license.php;	
//
using UnityEngine;
using System.Runtime.InteropServices;

public class MidiFighterTwister : MonoBehaviour
{
#if UNITY_64 || UNITY_EDITOR_64
    [DllImport("MidiFighterTwister64", EntryPoint = "mftStart")]
    public static extern byte mftStart();

    [DllImport("MidiFighterTwister64", EntryPoint = "mftStop")]
    public static extern void mftStop();

    [DllImport("MidiFighterTwister64", EntryPoint = "mftIsUpdatedKnobValue")]
    public static extern byte mftIsUpdatedKnobValue(byte idx);

    [DllImport("MidiFighterTwister64", EntryPoint = "mftGetKnobValue")]
    public static extern byte mftGetKnobValue(byte idx);

    [DllImport("MidiFighterTwister64", EntryPoint = "mftSetKnobValue")]
    public static extern void mftSetKnobValue(byte idx, byte value);

    [DllImport("MidiFighterTwister64", EntryPoint = "mftIsUpdatedButtonValue")]
    public static extern byte mftIsUpdatedButtonValue(byte idx);

    [DllImport("MidiFighterTwister64", EntryPoint = "mftGetButtonValue")]
    public static extern byte mftGetButtonValue(byte idx);

    [DllImport("MidiFighterTwister64", EntryPoint = "mftSetKnobColor")]
    public static extern void mftSetKnobColor(byte idx, byte value);

    [DllImport("MidiFighterTwister64", EntryPoint = "mftIsUpdatedBank")]
    public static extern byte mftIsUpdatedBank();

    [DllImport("MidiFighterTwister64", EntryPoint = "mftGetBank")]
    public static extern byte mftGetBank();

    [DllImport("MidiFighterTwister64", EntryPoint = "mftSetBank")]
    public static extern void mftSetBank(byte bank_idx);

    [DllImport("MidiFighterTwister64", EntryPoint = "mftSendCCMessage")]
    public static extern void mftSendCCMessage(byte channel, byte control_number, byte control_data);
#elif  UNITY_32 || UNITY_EDITOR_64
    [DllImport("MidiFighterTwister32", EntryPoint = "mftStart")]
    public static extern byte mftStart();

    [DllImport("MidiFighterTwister32", EntryPoint = "mftStop")]
    public static extern void mftStop();

    [DllImport("MidiFighterTwister32", EntryPoint = "mftIsUpdatedKnobValue")]
    public static extern byte mftIsUpdatedKnobValue(byte idx);

    [DllImport("MidiFighterTwister32", EntryPoint = "mftGetKnobValue")]
    public static extern byte mftGetKnobValue(byte idx);

    [DllImport("MidiFighterTwister32", EntryPoint = "mftSetKnobValue")]
    public static extern void mftSetKnobValue(byte idx, byte value);

    [DllImport("MidiFighterTwister32", EntryPoint = "mftIsUpdatedButtonValue")]
    public static extern byte mftIsUpdatedButtonValue(byte idx);

    [DllImport("MidiFighterTwister32", EntryPoint = "mftGetButtonValue")]
    public static extern byte mftGetButtonValue(byte idx);

    [DllImport("MidiFighterTwister32", EntryPoint = "mftSetKnobColor")]
    public static extern void mftSetKnobColor(byte idx, byte value);

    [DllImport("MidiFighterTwister32", EntryPoint = "mftIsUpdatedBank")]
    public static extern byte mftIsUpdatedBank();

    [DllImport("MidiFighterTwister32", EntryPoint = "mftGetBank")]
    public static extern byte mftGetBank();

    [DllImport("MidiFighterTwister32", EntryPoint = "mftSetBank")]
    public static extern void mftSetBank(byte bank_idx);

    [DllImport("MidiFighterTwister32", EntryPoint = "mftSendCCMessage")]
    public static extern void mftSendCCMessage(byte channel, byte control_number, byte control_data);
#else

#endif

    public void Awake()
    {
        bool rv = mftStart() > 0 ? true : false;

        if (rv == false) {
            Debug.LogError("mftStart() failed...");
            return;
        }

        Debug.Log("mftStart() success");
    }

    public void OnDestroy()
    {
        mftStop();
    }

    public bool IsUpdatedKnobValue(byte idx)
    {
        return mftIsUpdatedKnobValue(idx) > 0 ? true : false;
    }

    public byte GetKnobValue(byte idx)
    {
        return mftGetKnobValue(idx);
    }

    public void SetKnobValue(byte idx, byte value)
    {
        mftSetKnobValue(idx, value);
    }

    public bool IsUpdatedButtonValue(byte idx)
    {
        return mftIsUpdatedButtonValue(idx) > 0 ? true : false;
    }

    public byte GetButtonValue(byte idx)
    {
        return mftGetButtonValue(idx);
    }

    public void SetKnobColor(byte idx, byte value)
    {
        mftSetKnobColor(idx, value);
    }

    public bool IsUpdatedBank()
    {
        return mftIsUpdatedBank() > 0 ? true : false;
    }

    public byte GetBank()
    {
        return mftGetBank();
    }

    public void SetBank(byte bank_idx)
    {
        mftSetBank(bank_idx);
    }

    public void SendCCMessage(byte channel, byte control_number, byte control_data)
    {
        mftSendCCMessage(channel, control_number, control_data);
    }
}

