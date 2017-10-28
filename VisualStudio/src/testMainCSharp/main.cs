using System;

class TestMain
{
    static void Main(string[] args)
    {
        MidiFighterTwister.Start();

        MidiFighterTwister.SetBank(0);
        MidiFighterTwister.SetKnobColor(0, 32);
        MidiFighterTwister.SetKnobValue(0, 63);

        for (int i = 0; i < 180 * 60; ++i)
        {
            for (int idx = 0; idx < 64; ++idx)
            {
                if (MidiFighterTwister.IsUpdatedKnobValue((byte)idx))
                {
                    Console.WriteLine(string.Format("knob idx={0} value={1}", idx, MidiFighterTwister.GetKnobValue((byte)idx)));
                }

                if (MidiFighterTwister.IsUpdatedButtonValue((byte)idx))
                {
                    Console.WriteLine(string.Format("button idx={0} value={1}", idx, MidiFighterTwister.GetButtonValue((byte)idx)));
                }
            }

            if (MidiFighterTwister.IsUpdatedBank())
            {
                Console.WriteLine(string.Format("bank idx={0}", MidiFighterTwister.GetBank()));
            }

            System.Threading.Thread.Sleep(15);
        }

    }
}

