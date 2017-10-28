using UnityEngine;
using UnityEngine.UI;

public class SampleScript : MonoBehaviour {

    public MidiFighterTwister twister;

    public Slider knob_value0;
    public Slider knob_value1;
    public Slider knob_value2;
    public Slider knob_value3;

    public Slider knob_color0;
    public Slider knob_color1;
    public Slider knob_color2;
    public Slider knob_color3;

    void Start () {
        twister.SetBank(0);

        twister.SetKnobValue(0, 0);
        twister.SetKnobValue(1, 0);
        twister.SetKnobValue(2, 0);
        twister.SetKnobValue(3, 0);

        knob_value0.value = 0;
        knob_value1.value = 0;
        knob_value2.value = 0;
        knob_value3.value = 0;

        twister.SetKnobColor(0, 0);
        twister.SetKnobColor(1, 0);
        twister.SetKnobColor(2, 0);
        twister.SetKnobColor(3, 0);

        knob_color0.value = 0;
        knob_color1.value = 0;
        knob_color2.value = 0;
        knob_color3.value = 0;
    }

    void Update () {

        // knob value control
        if (twister.IsUpdatedKnobValue(0))
        {
            knob_value0.value = twister.GetKnobValue(0);
        }
        else
        {
            twister.SetKnobValue(0, (byte)knob_value0.value);
        }

        if (twister.IsUpdatedKnobValue(1))
        {
            knob_value1.value = twister.GetKnobValue(1);
        }
        else
        {
            twister.SetKnobValue(1, (byte)knob_value1.value);
        }

        if (twister.IsUpdatedKnobValue(2))
        {
            knob_value2.value = twister.GetKnobValue(2);
        }
        else
        {
            twister.SetKnobValue(2, (byte)knob_value2.value);
        }

        if (twister.IsUpdatedKnobValue(3))
        {
            knob_value3.value = twister.GetKnobValue(3);
        }
        else
        {
            twister.SetKnobValue(3, (byte)knob_value3.value);
        }

        // knob color control
        twister.SetKnobColor(0, (byte)knob_color0.value);
        twister.SetKnobColor(1, (byte)knob_color1.value);
        twister.SetKnobColor(2, (byte)knob_color2.value);
        twister.SetKnobColor(3, (byte)knob_color3.value);
    }
}
